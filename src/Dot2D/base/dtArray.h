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

#ifndef __DT_ARRAY_H__
#define __DT_ARRAY_H__

#include "dtMacros.h"
#include "dtConfig.h"
#include "dtRef.h"

#include <cstdint>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

NS_DT_BEGIN

typedef struct _dtArray {
	int32_t num, max;
	Ref** arr;
} dtArray;

/** Allocates and initializes a new array with specified capacity */
dtArray* dtArrayNew(int32_t capacity);

/** Frees array after removing all remaining objects. Silently ignores nil arr. */
void dtArrayFree(dtArray*& arr);

/** Doubles array capacity */
void dtArrayDoubleCapacity(dtArray *arr);

/** Increases array capacity such that max >= num + extra. */
void dtArrayEnsureExtraCapacity(dtArray *arr, int32_t extra);

/** shrinks the array so the memory footprint corresponds with the number of items */
void dtArrayShrink(dtArray *arr);

/** Returns index of first occurrence of object, NSNotFound if object not found. */
int32_t dtArrayGetIndexOfObject(dtArray *arr, Ref* object);

/** Returns a Boolean value that indicates whether object is present in array. */
bool dtArrayContainsObject(dtArray *arr, Ref* object);

/** Appends an object. Behavior undefined if array doesn't have enough capacity. */
void dtArrayAppendObject(dtArray *arr, Ref* object);

/** Appends an object. Capacity of arr is increased if needed. */
void dtArrayAppendObjectWithResize(dtArray *arr, Ref* object);

/** Appends objects from plusArr to arr. 
 Behavior undefined if arr doesn't have enough capacity. */
void dtArrayAppendArray(dtArray *arr, dtArray *plusArr);

/** Appends objects from plusArr to arr. Capacity of arr is increased if needed. */
void dtArrayAppendArrayWithResize(dtArray *arr, dtArray *plusArr);

/** Inserts an object at index */
void dtArrayInsertObjectAtIndex(dtArray *arr, Ref* object, int32_t index);

/** Swaps two objects */
void dtArraySwapObjectsAtIndexes(dtArray *arr, int32_t index1, int32_t index2);

/** Removes all objects from arr */
void dtArrayRemoveAllObjects(dtArray *arr);

/** Removes object at specified index and pushes back all subsequent objects.
 Behavior undefined if index outside [0, num-1]. */
void dtArrayRemoveObjectAtIndex(dtArray *arr, int32_t index, bool releaseObj = true);

/** Removes object at specified index and fills the gap with the last object,
 thereby avoiding the need to push back subsequent objects.
 Behavior undefined if index outside [0, num-1]. */
void dtArrayFastRemoveObjectAtIndex(dtArray *arr, int32_t index);

void dtArrayFastRemoveObject(dtArray *arr, Ref* object);

/** Searches for the first occurrence of object and removes it. If object is not
 found the function has no effect. */
void dtArrayRemoveObject(dtArray *arr, Ref* object, bool releaseObj = true);

/** Removes from arr all objects in minusArr. For each object in minusArr, the
 first matching instance in arr will be removed. */
void dtArrayRemoveArray(dtArray *arr, dtArray *minusArr);

/** Removes from arr all objects in minusArr. For each object in minusArr, all
 matching instances in arr will be removed. */
void dtArrayFullRemoveArray(dtArray *arr, dtArray *minusArr);

// 
// // dtCArray for Values (c structures)

typedef struct _dtCArray {
    int32_t num, max;
    void** arr;
} dtCArray;

/** Allocates and initializes a new C array with specified capacity */
dtCArray* dtCArrayNew(int32_t capacity);

/** Frees C array after removing all remaining values. Silently ignores nil arr. */
void dtCArrayFree(dtCArray *arr);

/** Doubles C array capacity */
void dtCArrayDoubleCapacity(dtCArray *arr);

/** Increases array capacity such that max >= num + extra. */
void dtCArrayEnsureExtraCapacity(dtCArray *arr, int32_t extra);

/** Returns index of first occurrence of value, NSNotFound if value not found. */
int32_t dtCArrayGetIndexOfValue(dtCArray *arr, void* value);

/** Returns a Boolean value that indicates whether value is present in the C array. */
bool dtCArrayContainsValue(dtCArray *arr, void* value);

/** Inserts a value at a certain position. Behavior undefined if array doesn't have enough capacity */
void dtCArrayInsertValueAtIndex( dtCArray *arr, void* value, int32_t index);

/** Appends an value. Behavior undefined if array doesn't have enough capacity. */
void dtCArrayAppendValue(dtCArray *arr, void* value);

/** Appends an value. Capacity of arr is increased if needed. */
void dtCArrayAppendValueWithResize(dtCArray *arr, void* value);

/** Appends values from plusArr to arr. Behavior undefined if arr doesn't have
 enough capacity. */
void dtCArrayAppendArray(dtCArray *arr, dtCArray *plusArr);

/** Appends values from plusArr to arr. Capacity of arr is increased if needed. */
void dtCArrayAppendArrayWithResize(dtCArray *arr, dtCArray *plusArr);

/** Removes all values from arr */
void dtCArrayRemoveAllValues(dtCArray *arr);

/** Removes value at specified index and pushes back all subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */
void dtCArrayRemoveValueAtIndex(dtCArray *arr, int32_t index);

/** Removes value at specified index and fills the gap with the last value,
 thereby avoiding the need to push back subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */
void dtCArrayFastRemoveValueAtIndex(dtCArray *arr, int32_t index);

/** Searches for the first occurrence of value and removes it. If value is not found the function has no effect.
 @since v0.99.4
 */
void dtCArrayRemoveValue(dtCArray *arr, void* value);

/** Removes from arr all values in minusArr. For each Value in minusArr, the first matching instance in arr will be removed.
 @since v0.99.4
 */
void dtCArrayRemoveArray(dtCArray *arr, dtCArray *minusArr);

/** Removes from arr all values in minusArr. For each value in minusArr, all matching instances in arr will be removed.
 @since v0.99.4
 */
void dtCArrayFullRemoveArray(dtCArray *arr, dtCArray *minusArr);

NS_DT_END

#endif //__DT_ARRAY_H__