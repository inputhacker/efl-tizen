#ifndef __EINA_PROMISE_PRIVATE_H__
#define __EINA_PROMISE_PRIVATE_H__

#define ERROR_DISPATCH(_cbs, _ret, _value, ...)                       \
  do {                                                                \
     Eina_Error ERROR_DISPATCH__err;                                  \
     if (!(_cbs)->error) (_ret) = (_value);                           \
     else                                                             \
       {                                                              \
          eina_value_get(&(_value), &ERROR_DISPATCH__err);            \
          (_ret) = (_cbs)->error(__VA_ARGS__, ERROR_DISPATCH__err);   \
       }                                                              \
  } while (0)

#define EASY_FUTURE_DISPATCH(_ret, _value, _dead_future, _cbs, ...)   \
  do {                                                                  \
     if ((_value).type == EINA_VALUE_TYPE_ERROR) ERROR_DISPATCH((_cbs), (_ret), (_value), __VA_ARGS__); \
     else                                                               \
       {                                                                \
          if ((!(_cbs)->success_type) || ((_cbs)->success_type && (_value).type == (_cbs)->success_type)) \
            {                                                           \
               if (!(_cbs)->success) (_ret) = (_value);  /* pass thru */ \
               else (_ret) = (_cbs)->success(__VA_ARGS__, (_value));    \
            }                                                           \
          else                                                          \
            {                                                           \
               Eina_Value EASY_FUTURE_DISPATCH__err = EINA_VALUE_EMPTY; \
               ERR("Future %p, success cb: %p, expected success_type %p (%s), got %p (%s)", \
                   _dead_future, (_cbs)->success,              \
                   (_cbs)->success_type, eina_value_type_name_get((_cbs)->success_type), \
                   (_value).type, (_value).type ? eina_value_type_name_get((_value).type) : NULL); \
               if (eina_value_setup(&EASY_FUTURE_DISPATCH__err, EINA_VALUE_TYPE_ERROR)) eina_value_set(&EASY_FUTURE_DISPATCH__err, EINVAL); \
               ERROR_DISPATCH((_cbs), (_ret), EASY_FUTURE_DISPATCH__err, __VA_ARGS__); \
            }                                                           \
       }                                                                \
     if ((_cbs)->free) (_cbs)->free(__VA_ARGS__, _dead_future);         \
  } while(0)
#endif
