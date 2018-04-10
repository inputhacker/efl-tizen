#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Efl.h>
#include <assert.h>

#define MY_CLASS EFL_UI_VIEW_LIST_SEGARRAY_CLASS
#define MY_CLASS_NAME "Efl.Ui.View.List.SegArray"

#include "efl_ui_view_list_private.h"
#include "efl_ui_view_list_segarray.h"

static int _search_lookup_cb(Eina_Rbtree const* rbtree, const void* key, int length EINA_UNUSED, void* data EINA_UNUSED)
{
  Efl_Ui_View_List_SegArray_Node const* node = (void const*)rbtree;
  int index = *(int*)key;
  if(index < node->first)
    {
      return 1;
    }
  else if(index < node->first + node->length)
    {
      return 0;
    }
  else
    {
      return -1;
    }
}

static int _insert_lookup_cb(Eina_Rbtree const* rbtree, const void* key, int length EINA_UNUSED, void* data EINA_UNUSED)
{
  Efl_Ui_View_List_SegArray_Node const* node = (void const*)rbtree;
  int index = *(int*)key;
  if(index < node->first)
    {
      return 1;
    }
  else if(index < node->first + node->max)
    {
      return 0;
    }
  else
    {
       return -1;
    }
}

static Eina_Rbtree_Direction _rbtree_compare(Efl_Ui_View_List_SegArray_Node const* left,
                                             Efl_Ui_View_List_SegArray_Node const* right, void* data EINA_UNUSED)
{
  if(left->first < right->first)
    return EINA_RBTREE_LEFT;
  else
    return EINA_RBTREE_RIGHT;
}


static void
_free_node(Efl_Ui_View_List_SegArray_Node* node, void* data EINA_UNUSED)
{
   int i = 0;

   while (i < node->length)
     {
       free(node->pointers[i]);
       ++i;
     }

   free(node);
}

static Efl_Ui_View_List_SegArray_Node*
_alloc_node(Efl_Ui_View_List_SegArray_Data* pd, int first)
{
   Efl_Ui_View_List_SegArray_Node* node;
   node = calloc(1, sizeof(Efl_Ui_View_List_SegArray_Node) + pd->step_size*sizeof(Efl_Ui_View_List_LayoutItem*));
   node->first = first;
   node->max = pd->step_size;
   pd->root = (void*)eina_rbtree_inline_insert(EINA_RBTREE_GET(pd->root), EINA_RBTREE_GET(node),
                                                     EINA_RBTREE_CMP_NODE_CB(&_rbtree_compare), NULL);
   pd->node_count++;
   return node;
}

EOLIAN static void
_efl_ui_view_list_segarray_flush(Eo* obj EINA_UNUSED, Efl_Ui_View_List_SegArray_Data *pd)
{
   if (pd->root)
     eina_rbtree_delete(EINA_RBTREE_GET(pd->root), EINA_RBTREE_FREE_CB(_free_node), NULL);

   pd->root = NULL;
   pd->node_count = 0;
   pd->count = 0;
}

static Efl_Ui_View_List_LayoutItem* _create_item_partial(Efl_Model* model)
{
   Efl_Ui_View_List_LayoutItem* item = calloc(1, sizeof(Efl_Ui_View_List_LayoutItem));
   item->children = model;
   return item;
}

static Efl_Ui_View_List_LayoutItem* _create_item(Efl_Model* model, Efl_Ui_View_List_SegArray_Node* node, unsigned int index)
{
   Efl_Ui_View_List_LayoutItem* item =  _create_item_partial(model);
   item->index_offset = index - node->first;
   item->tree_node = node;
   return item;
}

EOLIAN static Efl_Ui_View_List_LayoutItem*
_efl_ui_view_list_segarray_remove(Eo* obj EINA_UNUSED, Efl_Ui_View_List_SegArray_Data *pd, int index)
{
   Efl_Ui_View_List_SegArray_Node *node;
   Efl_Ui_View_List_LayoutItem *item, *rt;
   Eina_Iterator* iterator;
   int offset;

   node = (void*)eina_rbtree_inline_lookup(EINA_RBTREE_GET(pd->root),
                                        &index, sizeof(index), &_insert_lookup_cb, NULL);
   if (!node) return NULL;

   offset = index - node->first;
   if (offset >= node->length) return NULL;

   rt = node->pointers[offset];
   pd->count--;
   node->length--;

   while (offset < node->length)
     {
       node->pointers[offset] = node->pointers[offset+1];
       item = node->pointers[offset];
       --item->index_offset;
       ++offset;
     }

   node = (void*)EINA_RBTREE_GET(node)->son[EINA_RBTREE_LEFT];
   iterator = eina_rbtree_iterator_infix((void*)node);
   while(eina_iterator_next(iterator, (void**)&node))
     node->first--;

   return rt;
}

static void
_efl_ui_view_list_segarray_insert_at_node(Efl_Ui_View_List_SegArray_Data* pd, int index, Efl_Ui_View_List_LayoutItem* item, Efl_Ui_View_List_SegArray_Node* node)
{
   Eina_Iterator* iterator;
   int pos;

   if(node && node->length != node->max && (index - node->first) <= node->length)
    {
       pos = index - node->first;
       item->tree_node = node;
       item->index_offset = pos;
       if(pos < node->length)
         {
            assert(node->length != node->max);

            memmove(&node->pointers[pos], &node->pointers[pos+1], sizeof(node->pointers[pos])*(node->length - pos));
            node->pointers[pos] = item;
            node->length++;
         }
       else
         {
            assert(pos == node->length);

            assert(node->length != node->max);
            node->pointers[pos] = item;
            node->length++;
         }
    }
  else
    {
       node = _alloc_node(pd, index);
       node->pointers[0] = item;
       node->length++;
       item->index_offset = 0;
       item->tree_node = node;
    }

   node = (void*)EINA_RBTREE_GET(node)->son[EINA_RBTREE_LEFT];
   iterator = eina_rbtree_iterator_infix((void*)node);
   while(eina_iterator_next(iterator, (void**)&node))
     {
       node->first++;
     }

   eina_iterator_free(iterator);
}


EOLIAN static void
_efl_ui_view_list_segarray_insert(Eo *obj EINA_UNUSED, Efl_Ui_View_List_SegArray_Data* pd, int index, Efl_Model* model)
{
  Efl_Ui_View_List_SegArray_Node* node, *next;
  Efl_Ui_View_List_LayoutItem* item;

  item = _create_item_partial(model);

  node = (void*)eina_rbtree_inline_lookup(EINA_RBTREE_GET(pd->root),
                                          &index, sizeof(index), &_insert_lookup_cb, NULL);
  if(node)
    {
      next = (void*)EINA_RBTREE_GET(node)->son[EINA_RBTREE_LEFT];
      if(next && next->first <= index)
        _efl_ui_view_list_segarray_insert_at_node(pd, index, item, next);
      else
        _efl_ui_view_list_segarray_insert_at_node(pd, index, item, node);
    }
  else
    _efl_ui_view_list_segarray_insert_at_node(pd, index, item, NULL);
}

EOLIAN static void
_efl_ui_view_list_segarray_insert_accessor(Eo *obj EINA_UNUSED, Efl_Ui_View_List_SegArray_Data* pd, int first, Eina_Accessor* accessor)
{
   int i;
   Efl_Model* children;

   EINA_ACCESSOR_FOREACH(accessor, i, children)
     {
        Efl_Ui_View_List_SegArray_Node *node;
        int idx = first + i;

        node = (void*)eina_rbtree_inline_lookup(EINA_RBTREE_GET(pd->root),
                                                &idx, sizeof(idx), &_insert_lookup_cb, NULL);
        if (!node)
          {
             node = _alloc_node(pd, idx);
          }

        assert(node->length < node->max);
        node->pointers[node->length] = _create_item(children, node, idx);
        node->length++;
        pd->count++;
     }
}

EOLIAN static int
_efl_ui_view_list_segarray_count(Eo *obj EINA_UNUSED, Efl_Ui_View_List_SegArray_Data* pd)
{
   return pd->count;
}

typedef struct _Efl_Ui_View_List_Segarray_Eina_Accessor
{
   Eina_Accessor vtable;
   Efl_Ui_View_List_SegArray_Data* segarray;
} Efl_Ui_View_List_Segarray_Eina_Accessor;

static Eina_Bool
_efl_ui_view_list_segarray_accessor_get_at(Efl_Ui_View_List_Segarray_Eina_Accessor* acc,
                                      int idx, void** data)
{
   Efl_Ui_View_List_SegArray_Node* node;
   node = (void*)eina_rbtree_inline_lookup(EINA_RBTREE_GET(acc->segarray->root),
                                           &idx, sizeof(idx), &_search_lookup_cb, NULL);
   if (node && (node->first <= idx && node->first + node->length > idx))
     {
         int i = idx - node->first;
         Efl_Ui_View_List_LayoutItem* item = node->pointers[i];
         *data = item;
         return EINA_TRUE;
     }
   return EINA_FALSE;
}

EOLIAN static void
_efl_ui_view_list_segarray_setup(Eo *obj EINA_UNUSED, Efl_Ui_View_List_SegArray_Data *pd, int size)
{
   pd->step_size = size;
}

EOLIAN static Eo *
_efl_ui_view_list_segarray_efl_object_constructor(Eo *obj, Efl_Ui_View_List_SegArray_Data *pd EINA_UNUSED)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));

   return obj;
}

EOLIAN static void
_efl_ui_view_list_segarray_efl_object_destructor(Eo *obj, Efl_Ui_View_List_SegArray_Data *pd)
{
   if (pd->root)
     eina_rbtree_delete(EINA_RBTREE_GET(pd->root), EINA_RBTREE_FREE_CB(_free_node), NULL);

   pd->root = NULL;
   efl_destructor(efl_super(obj, MY_CLASS));
}

static void*
_efl_ui_view_list_segarray_accessor_get_container(Efl_Ui_View_List_Segarray_Eina_Accessor* acc EINA_UNUSED)
{
  return NULL;
}

static void
_efl_ui_view_list_segarray_accessor_free(Efl_Ui_View_List_Segarray_Eina_Accessor* acc EINA_UNUSED)
{
   free(acc);
}

static void
_efl_ui_view_list_segarray_accessor_lock(Efl_Ui_View_List_Segarray_Eina_Accessor* acc EINA_UNUSED)
{
}

static void
_efl_ui_view_list_segarray_accessor_unlock(Efl_Ui_View_List_Segarray_Eina_Accessor* acc EINA_UNUSED)
{
}

static Eina_Accessor*
_efl_ui_view_list_segarray_accessor_clone(Efl_Ui_View_List_Segarray_Eina_Accessor* acc EINA_UNUSED)
{
   return &acc->vtable;
}

static void
_efl_ui_view_list_segarray_accessor_setup(Efl_Ui_View_List_Segarray_Eina_Accessor* acc, Efl_Ui_View_List_SegArray_Data* segarray)
{
   EINA_MAGIC_SET(&acc->vtable, EINA_MAGIC_ACCESSOR);
   acc->vtable.version = EINA_ACCESSOR_VERSION;
   acc->vtable.get_at = FUNC_ACCESSOR_GET_AT(_efl_ui_view_list_segarray_accessor_get_at);
   acc->vtable.get_container = FUNC_ACCESSOR_GET_CONTAINER(_efl_ui_view_list_segarray_accessor_get_container);
   acc->vtable.free = FUNC_ACCESSOR_FREE(_efl_ui_view_list_segarray_accessor_free);
   acc->vtable.lock = FUNC_ACCESSOR_LOCK(_efl_ui_view_list_segarray_accessor_lock);
   acc->vtable.unlock = FUNC_ACCESSOR_LOCK(_efl_ui_view_list_segarray_accessor_unlock);
   acc->vtable.clone = FUNC_ACCESSOR_CLONE(_efl_ui_view_list_segarray_accessor_clone);
   acc->segarray = segarray;
}

EOLIAN static Eina_Accessor*
_efl_ui_view_list_segarray_accessor_get(Eo *obj EINA_UNUSED, Efl_Ui_View_List_SegArray_Data* pd)
{
   Efl_Ui_View_List_Segarray_Eina_Accessor* acc = calloc(1, sizeof(Efl_Ui_View_List_Segarray_Eina_Accessor));
   _efl_ui_view_list_segarray_accessor_setup(acc, pd);
   return &acc->vtable;
}

typedef struct _Efl_Ui_View_List_Segarray_Node_Accessor
{
   Eina_Accessor vtable;
   Efl_Ui_View_List_SegArray_Data* segarray;
   Eina_Iterator* pre_iterator;
   Efl_Ui_View_List_SegArray_Node* current_node;
   int current_index;
} Efl_Ui_View_List_Segarray_Node_Accessor;

static Eina_Bool
_efl_ui_view_list_segarray_node_accessor_get_at(Efl_Ui_View_List_Segarray_Node_Accessor* acc,
                                      int idx, void** data)
{
   if(idx == acc->current_index && acc->current_node)
     {
        (*data) = acc->current_node;
     }
   else
     {
       if(acc->current_index >= idx || !acc->current_node)
         {
            eina_iterator_free(acc->pre_iterator);
            acc->pre_iterator = NULL;
            acc->current_node = NULL;
            acc->current_index = -1;
         }

       if(!acc->pre_iterator)
         acc->pre_iterator = eina_rbtree_iterator_infix((void*)acc->segarray->root);

       for(;acc->current_index != idx;++acc->current_index)
         {
         if(!eina_iterator_next(acc->pre_iterator, (void**)&acc->current_node))
           {
             --acc->current_index;
             return EINA_FALSE;
           }
         }
       (*data) = acc->current_node;
       return EINA_TRUE;
     }
   return EINA_FALSE;
}

static void*
_efl_ui_view_list_segarray_node_accessor_get_container(Efl_Ui_View_List_Segarray_Node_Accessor* acc EINA_UNUSED)
{
  return NULL;
}

static void
_efl_ui_view_list_segarray_node_accessor_free(Efl_Ui_View_List_Segarray_Node_Accessor* acc EINA_UNUSED)
{
   if (acc->pre_iterator)
     eina_iterator_free(acc->pre_iterator);
   free(acc);
}

static void
_efl_ui_view_list_segarray_node_accessor_lock(Efl_Ui_View_List_Segarray_Node_Accessor* acc EINA_UNUSED)
{
}

static void
_efl_ui_view_list_segarray_node_accessor_unlock(Efl_Ui_View_List_Segarray_Node_Accessor* acc EINA_UNUSED)
{
}

static Eina_Accessor*
_efl_ui_view_list_segarray_node_accessor_clone(Efl_Ui_View_List_Segarray_Node_Accessor* acc EINA_UNUSED)
{
   return &acc->vtable;
}

static void
_efl_ui_view_list_segarray_node_accessor_setup(Efl_Ui_View_List_Segarray_Node_Accessor* acc, Efl_Ui_View_List_SegArray_Data* segarray)
{
   EINA_MAGIC_SET(&acc->vtable, EINA_MAGIC_ACCESSOR);
   acc->vtable.version = EINA_ACCESSOR_VERSION;
   acc->vtable.get_at = FUNC_ACCESSOR_GET_AT(_efl_ui_view_list_segarray_node_accessor_get_at);
   acc->vtable.get_container = FUNC_ACCESSOR_GET_CONTAINER(_efl_ui_view_list_segarray_node_accessor_get_container);
   acc->vtable.free = FUNC_ACCESSOR_FREE(_efl_ui_view_list_segarray_node_accessor_free);
   acc->vtable.lock = FUNC_ACCESSOR_LOCK(_efl_ui_view_list_segarray_node_accessor_lock);
   acc->vtable.unlock = FUNC_ACCESSOR_LOCK(_efl_ui_view_list_segarray_node_accessor_unlock);
   acc->vtable.clone = FUNC_ACCESSOR_CLONE(_efl_ui_view_list_segarray_node_accessor_clone);
   acc->segarray = segarray;
   acc->pre_iterator = NULL;
   acc->current_index = -1;
   acc->current_node = NULL;
}

EOLIAN static Eina_Accessor*
_efl_ui_view_list_segarray_node_accessor_get(Eo *obj EINA_UNUSED, Efl_Ui_View_List_SegArray_Data* pd)
{
   Efl_Ui_View_List_Segarray_Node_Accessor* acc = calloc(1, sizeof(Efl_Ui_View_List_Segarray_Node_Accessor));
   _efl_ui_view_list_segarray_node_accessor_setup(acc, pd);
   return &acc->vtable;
}

int efl_ui_view_list_item_index_get(Efl_Ui_View_List_LayoutItem* item)
{
  Efl_Ui_View_List_SegArray_Node* node = item->tree_node;
  return item->index_offset + node->first;
}

#include "efl_ui_view_list_segarray.eo.c"
