/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2013 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2016 Chukong Technologies Inc.
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
Copyright (c) 2021      Yinbaiyuan

http://www.cocos2d-x.org
https://www.yinbaiyuan.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "dtArray.h"
#include "dtTypes.h"

NS_DT_BEGIN

dtArray* dtArrayNew(int32_t capacity)
{
	if (capacity == 0)
		capacity = 7;
	
	dtArray *arr = (dtArray*)malloc( sizeof(dtArray) );
	arr->num = 0;
	arr->arr =  (Ref**)calloc(capacity, sizeof(Ref*));
	arr->max = capacity;
	
	return arr;
}

/** Frees array after removing all remaining objects. Silently ignores nullptr arr. */
void dtArrayFree(dtArray*& arr)
{
    if( arr == nullptr ) 
    {
        return;
    }
	dtArrayRemoveAllObjects(arr);
	
	free(arr->arr);
	free(arr);

    arr = nullptr;
}

void dtArrayDoubleCapacity(dtArray *arr)
{
	arr->max *= 2;
	Ref** newArr = (Ref**)realloc( arr->arr, arr->max * sizeof(Ref*) );
	arr->arr = newArr;
}

void dtArrayEnsureExtraCapacity(dtArray *arr, int32_t extra)
{
	while (arr->max < arr->num + extra)
    {
		dtArrayDoubleCapacity(arr);
    }
}

void dtArrayShrink(dtArray *arr)
{
    int32_t newSize = 0;
	
	if (arr->max > arr->num && !(arr->num==0 && arr->max==1))
	{
		if (arr->num!=0)
		{
			newSize=arr->num;
			arr->max=arr->num;
		}
		else
		{
			newSize=1;
			arr->max=1;
		}
		
		arr->arr = (Ref**)realloc(arr->arr,newSize * sizeof(Ref*) );
	}
}

/** Returns index of first occurrence of object, DT_INVALID_INDEX if object not found. */
int32_t dtArrayGetIndexOfObject(dtArray *arr, Ref* object)
{
    const auto arrNum = arr->num;
    Ref** ptr = arr->arr;
	for (int32_t i = 0; i < arrNum; ++i, ++ptr)
    {
		if (*ptr == object)
            return i;
    }
    
	return DT_INVALID_INDEX;
}

/** Returns a Boolean value that indicates whether object is present in array. */
bool dtArrayContainsObject(dtArray *arr, Ref* object)
{
	return dtArrayGetIndexOfObject(arr, object) != DT_INVALID_INDEX;
}

/** Appends an object. Behavior undefined if array doesn't have enough capacity. */
void dtArrayAppendObject(dtArray *arr, Ref* object)
{
    object->retain();
	arr->arr[arr->num] = object;
	arr->num++;
}

/** Appends an object. Capacity of arr is increased if needed. */
void dtArrayAppendObjectWithResize(dtArray *arr, Ref* object)
{
	dtArrayEnsureExtraCapacity(arr, 1);
	dtArrayAppendObject(arr, object);
}

/** Appends objects from plusArr to arr. Behavior undefined if arr doesn't have
 enough capacity. */
void dtArrayAppendArray(dtArray *arr, dtArray *plusArr)
{
	for (int32_t i = 0; i < plusArr->num; i++)
    {
		dtArrayAppendObject(arr, plusArr->arr[i]);
    }
}

/** Appends objects from plusArr to arr. Capacity of arr is increased if needed. */
void dtArrayAppendArrayWithResize(dtArray *arr, dtArray *plusArr)
{
	dtArrayEnsureExtraCapacity(arr, plusArr->num);
	dtArrayAppendArray(arr, plusArr);
}

/** Inserts an object at index */
void dtArrayInsertObjectAtIndex(dtArray *arr, Ref* object, int32_t index)
{
	dtArrayEnsureExtraCapacity(arr, 1);
	
	int32_t remaining = arr->num - index;
	if (remaining > 0)
    {
		memmove((void *)&arr->arr[index+1], (void *)&arr->arr[index], sizeof(Ref*) * remaining );
    }

    object->retain();
	arr->arr[index] = object;
	arr->num++;
}

/** Swaps two objects */
void dtArraySwapObjectsAtIndexes(dtArray *arr, int32_t index1, int32_t index2)
{
	Ref* object1 = arr->arr[index1];
	
	arr->arr[index1] = arr->arr[index2];
	arr->arr[index2] = object1;
}

/** Removes all objects from arr */
void dtArrayRemoveAllObjects(dtArray *arr)
{
	while (arr->num > 0)
    {
		(arr->arr[--arr->num])->release();
    }
}

/** Removes object at specified index and pushes back all subsequent objects.
 Behavior undefined if index outside [0, num-1]. */
void dtArrayRemoveObjectAtIndex(dtArray *arr, int32_t index, bool releaseObj/* = true*/)
{
    if (releaseObj)
    {
        DT_SAFE_RELEASE(arr->arr[index]);
    }
    
	arr->num--;
	
	int32_t remaining = arr->num - index;
	if(remaining>0)
    {
		memmove((void *)&arr->arr[index], (void *)&arr->arr[index+1], remaining * sizeof(Ref*));
    }
}

/** Removes object at specified index and fills the gap with the last object,
 thereby avoiding the need to push back subsequent objects.
 Behavior undefined if index outside [0, num-1]. */
void dtArrayFastRemoveObjectAtIndex(dtArray *arr, int32_t index)
{
	DT_SAFE_RELEASE(arr->arr[index]);
	auto last = --arr->num;
	arr->arr[index] = arr->arr[last];
}

void dtArrayFastRemoveObject(dtArray *arr, Ref* object)
{
	auto index = dtArrayGetIndexOfObject(arr, object);
	if (index != DT_INVALID_INDEX)
    {
		dtArrayFastRemoveObjectAtIndex(arr, index);
    }
}

/** Searches for the first occurrence of object and removes it. If object is not
 found the function has no effect. */
void dtArrayRemoveObject(dtArray *arr, Ref* object, bool releaseObj/* = true*/)
{
	auto index = dtArrayGetIndexOfObject(arr, object);
	if (index != DT_INVALID_INDEX)
    {
		dtArrayRemoveObjectAtIndex(arr, index, releaseObj);
    }
}

/** Removes from arr all objects in minusArr. For each object in minusArr, the
 first matching instance in arr will be removed. */
void dtArrayRemoveArray(dtArray *arr, dtArray *minusArr)
{
	for (int32_t i = 0; i < minusArr->num; i++)
    {
		dtArrayRemoveObject(arr, minusArr->arr[i]);
    }
}

/** Removes from arr all objects in minusArr. For each object in minusArr, all
 matching instances in arr will be removed. */
void dtArrayFullRemoveArray(dtArray *arr, dtArray *minusArr)
{
	int32_t back = 0;
	
	for (int32_t i = 0; i < arr->num; i++)
    {
		if (dtArrayContainsObject(minusArr, arr->arr[i]))
        {
			DT_SAFE_RELEASE(arr->arr[i]);
			back++;
		} 
        else
        {
			arr->arr[i - back] = arr->arr[i];
        }
	}
	
	arr->num -= back;
}

// 
// // dtCArray for Values (c structures)

/** Allocates and initializes a new C array with specified capacity */
dtCArray* dtCArrayNew(int32_t capacity)
{
	if (capacity == 0)
    {
		capacity = 7;
    }

	dtCArray *arr = (dtCArray*)malloc(sizeof(dtCArray));
	arr->num = 0;
	arr->arr = (void**)malloc(capacity * sizeof(void*));
	arr->max = capacity;
	
	return arr;
}

/** Frees C array after removing all remaining values. Silently ignores nullptr arr. */
void dtCArrayFree(dtCArray *arr)
{
    if (arr == nullptr)
    {
        return;
    }
	dtCArrayRemoveAllValues(arr);
	
	free(arr->arr);
	free(arr);
}

/** Doubles C array capacity */
void dtCArrayDoubleCapacity(dtCArray *arr)
{
    dtArrayDoubleCapacity((dtArray*)arr);
}

/** Increases array capacity such that max >= num + extra. */
void dtCArrayEnsureExtraCapacity(dtCArray *arr, int32_t extra)
{
    dtArrayEnsureExtraCapacity((dtArray*)arr,extra);
}

/** Returns index of first occurrence of value, DT_INVALID_INDEX if value not found. */
int32_t dtCArrayGetIndexOfValue(dtCArray *arr, void* value)
{
	for(int32_t i = 0; i < arr->num; i++)
    {
		if( arr->arr[i] == value )
            return i;
    }
	return DT_INVALID_INDEX;
}

/** Returns a Boolean value that indicates whether value is present in the C array. */
bool dtCArrayContainsValue(dtCArray *arr, void* value)
{
	return dtCArrayGetIndexOfValue(arr, value) != DT_INVALID_INDEX;
}

/** Inserts a value at a certain position. Behavior undefined if array doesn't have enough capacity */
void dtCArrayInsertValueAtIndex( dtCArray *arr, void* value, int32_t index)
{
	
	auto remaining = arr->num - index;
    // make sure it has enough capacity
    if (arr->num + 1 == arr->max)
    {
        dtCArrayDoubleCapacity(arr);
    }
	// last Value doesn't need to be moved
	if( remaining > 0) {
		// tex coordinates
		memmove((void *)&arr->arr[index+1], (void *)&arr->arr[index], sizeof(void*) * remaining );
	}
	
	arr->num++;
	arr->arr[index] = value;
}

/** Appends an value. Behavior undefined if array doesn't have enough capacity. */
void dtCArrayAppendValue(dtCArray *arr, void* value)
{
	arr->arr[arr->num] = value;
	arr->num++;
    // double the capacity for the next append action
    // if the num >= max
    if (arr->num >= arr->max)
    {
        dtCArrayDoubleCapacity(arr);
    }
}

/** Appends an value. Capacity of arr is increased if needed. */
void dtCArrayAppendValueWithResize(dtCArray *arr, void* value)
{
	dtCArrayEnsureExtraCapacity(arr, 1);
	dtCArrayAppendValue(arr, value);
}


/** Appends values from plusArr to arr. Behavior undefined if arr doesn't have
 enough capacity. */
void dtCArrayAppendArray(dtCArray *arr, dtCArray *plusArr)
{
	for( int32_t i = 0; i < plusArr->num; i++)
    {
		dtCArrayAppendValue(arr, plusArr->arr[i]);
    }
}

/** Appends values from plusArr to arr. Capacity of arr is increased if needed. */
void dtCArrayAppendArrayWithResize(dtCArray *arr, dtCArray *plusArr)
{
	dtCArrayEnsureExtraCapacity(arr, plusArr->num);
	dtCArrayAppendArray(arr, plusArr);
}

/** Removes all values from arr */
void dtCArrayRemoveAllValues(dtCArray *arr)
{
	arr->num = 0;
}

/** Removes value at specified index and pushes back all subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */
void dtCArrayRemoveValueAtIndex(dtCArray *arr, int32_t index)
{
	for( int32_t last = --arr->num; index < last; index++)
    {
		arr->arr[index] = arr->arr[index + 1];
    }
}

/** Removes value at specified index and fills the gap with the last value,
 thereby avoiding the need to push back subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */
void dtCArrayFastRemoveValueAtIndex(dtCArray *arr, int32_t index)
{
	int32_t last = --arr->num;
	arr->arr[index] = arr->arr[last];
}

/** Searches for the first occurrence of value and removes it. If value is not found the function has no effect.
 @since v0.99.4
 */
void dtCArrayRemoveValue(dtCArray *arr, void* value)
{
	auto index = dtCArrayGetIndexOfValue(arr, value);
	if (index != DT_INVALID_INDEX)
    {
		dtCArrayRemoveValueAtIndex(arr, index);
    }
}

/** Removes from arr all values in minusArr. For each Value in minusArr, the first matching instance in arr will be removed.
 @since v0.99.4
 */
void dtCArrayRemoveArray(dtCArray *arr, dtCArray *minusArr)
{
	for(int32_t i = 0; i < minusArr->num; i++)
    {
		dtCArrayRemoveValue(arr, minusArr->arr[i]);
    }
}

/** Removes from arr all values in minusArr. For each value in minusArr, all matching instances in arr will be removed.
 @since v0.99.4
 */
void dtCArrayFullRemoveArray(dtCArray *arr, dtCArray *minusArr)
{
	int32_t back = 0;
	
	for(int32_t i = 0; i < arr->num; i++)
    {
		if( dtCArrayContainsValue(minusArr, arr->arr[i]) ) 
        {
			back++;
		} 
        else
        {
			arr->arr[i - back] = arr->arr[i];
        }
	}
	
	arr->num -= back;
}

NS_DT_END