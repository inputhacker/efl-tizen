using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;


using static eina.EinaNative.PromiseNativeMethods;

namespace eina {

namespace EinaNative {

static internal class PromiseNativeMethods
{
    internal delegate void Promise_Cancel_Cb(IntPtr data, IntPtr dead);

    [DllImport(efl.Libs.Ecore)]
    internal static extern IntPtr efl_loop_promise_new(IntPtr obj, Promise_Cancel_Cb cancel_cb, IntPtr data);

    [DllImport(efl.Libs.Eina)]
    internal static extern IntPtr eina_promise_new(IntPtr scheduler, Promise_Cancel_Cb cancel_cb, IntPtr data);

    [DllImport(efl.Libs.Eina)]
    internal static extern void eina_promise_resolve(IntPtr scheduler, eina.Value_Native value);

    [DllImport(efl.Libs.Eina)]
    internal static extern void eina_promise_reject(IntPtr scheduler, eina.Error reason);

    [DllImport(efl.Libs.Eina)]
    internal static extern IntPtr eina_future_new(IntPtr promise);

    [DllImport(efl.Libs.Eina)]
    internal static extern void eina_future_cancel(IntPtr future);

    [DllImport(efl.Libs.Ecore)]
    internal static extern IntPtr efl_loop_future_scheduler_get(IntPtr obj);

    [DllImport(efl.Libs.Eina)]
    internal static extern IntPtr eina_future_then_from_desc(IntPtr prev, FutureDesc desc);

    [DllImport(efl.Libs.Eina)]
    internal static extern IntPtr eina_future_chain_array(IntPtr prev, FutureDesc[] desc);

    internal delegate eina.Value_Native FutureCb(IntPtr data, eina.Value_Native value, IntPtr dead_future);

    [StructLayout(LayoutKind.Sequential)]
    internal struct FutureDesc
    {
        internal FutureCb cb;
        internal IntPtr data;
        internal IntPtr storage; // Internal use by eina

        public FutureDesc(FutureCb cb, IntPtr data, IntPtr storage)
        {
            this.cb = cb;
            this.data = data;
            this.storage = storage;
        }
    }
}

} // namespace EinaNative

/// <summary>
/// Promises act as placeholders for a value that may be available in the future.
///
/// With a Promise you can attach futures to it, which will be used to notify of the value being available.
/// </summary>
public class Promise
{
    internal IntPtr Handle;
    private GCHandle CleanupHandle;

    /// <summary>Delegate for functions that will be called upon a promise cancellation.</summary>
    public delegate void CancelCb();

    /// <summary>
    /// Creates a new Promise with the given callback.
    ///
    /// Currently, creating a promise directly uses the Main Loop scheduler the source of notifications (i.e. the
    /// future callbacks will be called mainly from a loop iteration).
    /// </summary>
    public Promise(CancelCb cancelCb=null)
    {
        efl.ILoop loop = efl.App.GetLoopMain();

        // Should we be able to pass different schedulers?
        IntPtr scheduler = efl_loop_future_scheduler_get(loop.raw_handle);

        IntPtr cb_data = IntPtr.Zero;

        // A safety clean callback to mark this wrapper as invalid
        CancelCb safetyCb = () => {
            Handle = IntPtr.Zero;
            if (cancelCb != null)
                cancelCb();
        };

        CleanupHandle = GCHandle.Alloc(safetyCb);
        cb_data = GCHandle.ToIntPtr(CleanupHandle);

        this.Handle = eina_promise_new(scheduler, NativeCancelCb, cb_data);
    }

    private static void NativeCancelCb(IntPtr data, IntPtr dead)
    {
        if (data == IntPtr.Zero)
            return;

        GCHandle handle = GCHandle.FromIntPtr(data);
        CancelCb cb = handle.Target as CancelCb;
        if (cb != null)
            cb();
        else
            eina.Log.Info("Null promise CancelCb found");
        handle.Free();
    }

    private void SanityChecks()
    {
        if (this.Handle == IntPtr.Zero)
            throw new ObjectDisposedException(GetType().Name);
    }

    /// <summary>
    /// Fulfills a promise with the given value.
    ///
    /// This will make all futures attached to it to be called with the given value as payload.
    /// </summary>
    public void Resolve(eina.Value value)
    {
        SanityChecks();
        eina_promise_resolve(this.Handle, value);
        this.Handle = IntPtr.Zero;
        // Resolving a cb does *not* call its cancellation callback, so we have to release the
        // lambda created in the constructor for cleanup.
        CleanupHandle.Free();
    }

    /// <summary>
    /// Rejects a promise.
    ///
    /// The future chain attached to this promise will be called with an eina.Value of type
    /// eina.ValueType.Error and payload eina.Error.ECANCELED.
    /// </summary>
    public void Reject(eina.Error reason)
    {
        SanityChecks();
        eina_promise_reject(this.Handle, reason);
        this.Handle = IntPtr.Zero;
    }
}

/// <summary>
/// Futures are the structures holding the callbacks to be notified of a promise fullfillment
/// or cancellation.
/// </summary>
public class Future
{
    /// <summary>
    /// Callback attached to a future and to be called when resolving/rejecting a promise.
    ///
    /// The eina.Value as argument can come with an eina.Error.ECANCELED as payload if the
    /// promise/future was rejected/cancelled.
    ///
    /// The return value usually is same as the argument, forwarded, but can be changed in
    /// case were the chain act as a transforming pipeline.
    /// </summary>
    public delegate eina.Value ResolvedCb(eina.Value value);

    public IntPtr Handle { get; internal set; }

    /// <summary>
    /// Creates a Future from a native pointer.
    /// </summary>
    public Future(IntPtr handle)
    {
        Handle = ThenRaw(handle, (eina.Value value) => {
            Handle = IntPtr.Zero;
            return value;
        });
    }

    /// <summary>
    /// Creates a Future attached to the given Promise.
    ///
    /// Optionally a resolved callback may be provided. If so, it will be chained
    /// before the returned future.
    /// </summary>
    public Future(Promise promise, ResolvedCb cb=null)
    {
        IntPtr intermediate = eina_future_new(promise.Handle);
        Handle = ThenRaw(intermediate, (eina.Value value) => {
            if (cb != null)
                value = cb(value);
            Handle = IntPtr.Zero;
            return value;
        });
    }

    private void SanityChecks()
    {
        if (this.Handle == IntPtr.Zero)
            throw new ObjectDisposedException(GetType().Name);
    }

    /// <summary>
    /// Cancels this future and the chain it belongs to, along with the promise linked against it.
    ///
    /// The callbacks will still be called with eina.Error.ECANCELED as payload. The promise cancellation
    /// callback will also be called if present.
    /// </summary>
    public void Cancel()
    {
        SanityChecks();
        eina_future_cancel(this.Handle);
    }

    /// <summary>
    /// Creates a new future to be called after this one.
    ///
    /// Once the promise this future is attached to resolves, the callbacks on the chain
    /// are called in the order they were chained.
    ///
    /// CAUTION: Calling Then() on a future that had it called before will replace the previous chain
    /// from this point on.
    /// </summary>
    public Future Then(ResolvedCb cb)
    {
        SanityChecks();
        return new Future(ThenRaw(Handle, cb));
    }

    // Helper function to attach a cb to a future without creating a new wrapper directly.
    // It'll be used in the construtor, to attach the cleaning cb to an intermediate future.
    private static IntPtr ThenRaw(IntPtr previous, ResolvedCb cb)
    {
        FutureDesc desc = new FutureDesc();
        desc.cb = NativeResolvedCb;
        GCHandle handle = GCHandle.Alloc(cb);
        desc.data = GCHandle.ToIntPtr(handle);
        return eina_future_then_from_desc(previous, desc);
    }
    private static eina.Value_Native NativeResolvedCb(IntPtr data, eina.Value_Native value, IntPtr dead_future)
    {
        GCHandle handle = GCHandle.FromIntPtr(data);
        ResolvedCb cb = handle.Target as ResolvedCb;
        if (cb != null)
            value = cb(value);
        else
            eina.Log.Warning("Failed to get future callback.");
        handle.Free();
        return value;
    }

    /// <summary>
    /// Helper method for chaining a group of callbacks in a single go.
    ///
    /// It is just syntatic sugar for sequential Then() calls, without creating intermediate
    /// futures explicitly.
    /// </summary>
    public Future Chain(IEnumerable<ResolvedCb> cbs)
    {
        SanityChecks();
        System.Collections.Generic.IList<ResolvedCb> cbsList = cbs.ToList();
        FutureDesc[] descs = new FutureDesc[cbsList.Count() + 1]; // +1 due to the null-cb terminating descriptor.
        int i = 0;
        try
        {
            for (; i < cbsList.Count(); i++)
            {
                ResolvedCb cb = cbsList[i];
                descs[i].cb = NativeResolvedCb;
                GCHandle handle = GCHandle.Alloc(cb);
                descs[i].data = GCHandle.ToIntPtr(handle);
            }

            descs[i].cb = null;
            descs[i].data = IntPtr.Zero;
        }
        catch (Exception e)
        {
            for (int j = 0; j <= i; j++)
            {
                if (descs[i].data == IntPtr.Zero)
                    continue;

                GCHandle handle = GCHandle.FromIntPtr(descs[i].data);
                handle.Free();
            }
            eina.Log.Error($"Failed to create native future description for callbacks. Error: {e.ToString()}");
            return null;
        }
        return new Future(eina_future_chain_array(Handle, descs));
    }
}

public class FutureMarshaler : ICustomMarshaler
{

    public object MarshalNativeToManaged(IntPtr pNativeData)
    {
        return new Future(pNativeData);
    }

    public IntPtr MarshalManagedToNative(object managedObj)
    {
        Future f = managedObj as Future;
        if (f == null)
            return IntPtr.Zero;
        return f.Handle;
    }

    public void CleanUpNativeData(IntPtr pNativeData) { }

    public void CleanUpManagedData(object managedObj) { }

    public int GetNativeDataSize()
    {
        return -1;
    }

    public static ICustomMarshaler GetInstance(string cookie) {
        if (marshaler == null)
        {
            marshaler = new FutureMarshaler();
        }
        return marshaler;
    }

    private static FutureMarshaler marshaler;
}

} // namespace eina
