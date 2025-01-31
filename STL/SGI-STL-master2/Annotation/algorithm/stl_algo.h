/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef __SGI_STL_INTERNAL_ALGO_H
#define __SGI_STL_INTERNAL_ALGO_H

#include <stl_heap.h>

__STL_BEGIN_NAMESPACE

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1209
#endif
// 返回 a b c 中居中者
template <class T>
inline const T& __median(const T& a, const T& b, const T& c) {
  if (a < b)
    if (b < c)
      return b;
    else if (a < c)
      return c;
    else
      return a;
  else if (a < c)
    return a;
  else if (b < c)
    return c;
  else
    return b;
}

template <class T, class Compare>
inline const T& __median(const T& a, const T& b, const T& c, Compare comp) {
  if (comp(a, b))
    if (comp(b, c))
      return b;
    else if (comp(a, c))
      return c;
    else
      return a;
  else if (comp(a, c))
    return a;
  else if (comp(b, c))
    return c;
  else
    return b;
}

//为每一个元素执行一个动作(function)
template <class InputIterator, class Function>
Function for_each(InputIterator first, InputIterator last, Function f) {
  for ( ; first != last; ++first)
    f(*first);
  return f;
}

/*
  查找 ，关联性容器(包括无序）有内置更快的做法.
  因为根本的底层结构不同
*/
template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
  while (first != last && *first != value) ++first;
  return first;
}

template <class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last,
                      Predicate pred) {
  while (first != last && !pred(*first)) ++first;
  return first;
}
/*
*  找到第一组满足条件(默认是相等)的相邻元素
*/
//版本一
template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
  if (first == last) return last;
  ForwardIterator next = first;
  while(++next != last) {
    if (*first == *next) return first;
    first = next;
  }
  return last;
}
//版本二
template <class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,
                              BinaryPredicate binary_pred) {
  if (first == last) return last;
  ForwardIterator next = first;
  while(++next != last) {
    if (binary_pred(*first, *next)) return first;
    first = next;
  }
  return last;
}
//记数 等于 value 的个数 ,set(multi),map(multi)自带的有count函数
template <class InputIterator, class T, class Size>
void count(InputIterator first, InputIterator last, const T& value,
           Size& n) {
  for ( ; first != last; ++first)
    if (*first == value)
      ++n;
}
//满足条件 pred的个数
template <class InputIterator, class Predicate, class Size>
void count_if(InputIterator first, InputIterator last, Predicate pred,
              Size& n) {
  for ( ; first != last; ++first)
    if (pred(*first))
      ++n;
}

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
// 其他版本 ，之前 n 由参数提供
template <class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value) {
  typename iterator_traits<InputIterator>::difference_type n = 0;
  for ( ; first != last; ++first)
    if (*first == value)
      ++n;
  return n;
}

template <class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, Predicate pred) {
  typename iterator_traits<InputIterator>::difference_type n = 0;
  for ( ; first != last; ++first)
    if (pred(*first))
      ++n;
  return n;
}


#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */
// 在序列1 [frirst1, last1) 的区间内,查找整个序列2 [first2, last2)的首次出现点
template <class ForwardIterator1, class ForwardIterator2, class Distance1,
          class Distance2>
ForwardIterator1 __search(ForwardIterator1 first1, ForwardIterator1 last1,
                          ForwardIterator2 first2, ForwardIterator2 last2,
                          Distance1*, Distance2*) {
  Distance1 d1 = 0;
  distance(first1, last1, d1);
  Distance2 d2 = 0;
  distance(first2, last2, d2);

  if (d1 < d2) return last1; //如果序列2大于序列1,那么不可能出现其子序列

  ForwardIterator1 current1 = first1;
  ForwardIterator2 current2 = first2;

  while (current2 != last2) 
    if (*current1 == *current2) {
      ++current1;
      ++current2;
    }
    else {
      if (d1 == d2)
        return last1;
      else {
        current1 = ++first1;
        current2 = first2;
        --d1;
      }
    }
  return first1;
}

template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                               ForwardIterator2 first2, ForwardIterator2 last2)
{
  return __search(first1, last1, first2, last2, distance_type(first1),
                  distance_type(first2));
}

template <class ForwardIterator1, class ForwardIterator2,
          class BinaryPredicate, class Distance1, class Distance2>
ForwardIterator1 __search(ForwardIterator1 first1, ForwardIterator1 last1,
                          ForwardIterator2 first2, ForwardIterator2 last2,
                          BinaryPredicate binary_pred, Distance1*, Distance2*) {
  Distance1 d1 = 0;
  distance(first1, last1, d1);
  Distance2 d2 = 0;
  distance(first2, last2, d2);

  if (d1 < d2) return last1;

  ForwardIterator1 current1 = first1;
  ForwardIterator2 current2 = first2;

  while (current2 != last2)
    if (binary_pred(*current1, *current2)) {
      ++current1;
      ++current2;
    }
    else {
      if (d1 == d2)
        return last1;
      else {
        current1 = ++first1;
        current2 = first2;
        --d1;
      }
    }
  return first1;
}

template <class ForwardIterator1, class ForwardIterator2,
          class BinaryPredicate>
inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                               ForwardIterator2 first2, ForwardIterator2 last2,
                               BinaryPredicate binary_pred) {
  return __search(first1, last1, first2, last2, binary_pred,
                  distance_type(first1), distance_type(first2));
}
// 在第一个序列上,查找"连续 count个符合条件之元素"所形成的子序列
template <class ForwardIterator, class Integer, class T>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
                         Integer count, const T& value) {
  if (count <= 0)
    return first;
  else {
    first = find(first, last, value);
    while (first != last) {
      Integer n = count - 1;
      ForwardIterator i = first;
      ++i;
      while (i != last && n != 0 && *i == value) {
        ++i;
        --n;
      }
      if (n == 0)
        return first;
      else
        first = find(i, last, value);
    }
    return last;
  }
}

template <class ForwardIterator, class Integer, class T, class BinaryPredicate>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
                         Integer count, const T& value,
                         BinaryPredicate binary_pred) {
  if (count <= 0)
    return first;
  else {
    while (first != last) {
      if (binary_pred(*first, value)) break;
      ++first;
    }
    while (first != last) {
      Integer n = count - 1;
      ForwardIterator i = first;
      ++i;
      while (i != last && n != 0 && binary_pred(*i, value)) {
        ++i;
        --n;
      }
      if (n == 0)
        return first;
      else {
        while (i != last) {
          if (binary_pred(*i, value)) break;
          ++i;
        }
        first = i;
      }
    }
    return last;
  }
} 
//将第一个区间内的元素与从 "first2开始"等长的元素互相交换
template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
                             ForwardIterator2 first2) {
  for ( ; first1 != last1; ++first1, ++first2)
    iter_swap(first1, first2);
  return first2;
}
//以 op 作用于区间中的每一个元素
template <class InputIterator, class OutputIterator, class UnaryOperation>
OutputIterator transform(InputIterator first, InputIterator last,
                         OutputIterator result, UnaryOperation op) {
  for ( ; first != last; ++first, ++result)
    *result = op(*first);
  return result;
}
// op作用于 序列1和序列2 组成的元素
template <class InputIterator1, class InputIterator2, class OutputIterator,
          class BinaryOperation>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, OutputIterator result,
                         BinaryOperation binary_op) {
  for ( ; first1 != last1; ++first1, ++first2, ++result)
    *result = binary_op(*first1, *first2);
  return result;
}
//用新值替代旧值
template <class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last, const T& old_value,
             const T& new_value) {
  for ( ; first != last; ++first)
    if (*first == old_value) *first = new_value;
}
//如果满足Pred，则用新值覆盖
template <class ForwardIterator, class Predicate, class T>
void replace_if(ForwardIterator first, ForwardIterator last, Predicate pred,
                const T& new_value) {
  for ( ; first != last; ++first)
    if (pred(*first)) *first = new_value;
}
//范围内所有等同于 旧值 都以新值取代，不符合者原值放入新区间,并返回
template <class InputIterator, class OutputIterator, class T>
OutputIterator replace_copy(InputIterator first, InputIterator last,
                            OutputIterator result, const T& old_value,
                            const T& new_value) {
  for ( ; first != last; ++first, ++result)
    *result = *first == old_value ? new_value : *first;
  return result;
}

template <class Iterator, class OutputIterator, class Predicate, class T>
OutputIterator replace_copy_if(Iterator first, Iterator last,
                               OutputIterator result, Predicate pred,
                               const T& new_value) {
  for ( ; first != last; ++first, ++result)
    *result = pred(*first) ? new_value : *first;
  return result;
}
//将仿函数 结果填写在[first, last)区间的所有元素身上
template <class ForwardIterator, class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
  for ( ; first != last; ++first)
    *first = gen();
}

template <class OutputIterator, class Size, class Generator>
OutputIterator generate_n(OutputIterator first, Size n, Generator gen) {
  for ( ; n > 0; --n, ++first)
    *first = gen();
  return first;
}

/*
* 移除与 value 相等的元素(但不改变原容器删除),而是将结果复制到一个新区间上
*/
template <class InputIterator, class OutputIterator, class T>
OutputIterator remove_copy(InputIterator first, InputIterator last,
                           OutputIterator result, const T& value) {
  for ( ; first != last; ++first)
    if (*first != value) {
      *result = *first;
      ++result;
    }
  return result;
}

template <class InputIterator, class OutputIterator, class Predicate>
OutputIterator remove_copy_if(InputIterator first, InputIterator last,
                              OutputIterator result, Predicate pred) {
  for ( ; first != last; ++first)
    if (!pred(*first)) {
      *result = *first;
      ++result;
    }
  return result;
}
/*
* 移除与 value 相等的元素(但不删除),利用 remove_copy 将[first+1,last）位置上的元素移动到[first,last) 
* 对 array 使用时，因为 array 无法改变区间,会导致残余的元素一直存在
*/
template <class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last,
                       const T& value) {
  first = find(first, last, value); //找到第一个相等的元素
  ForwardIterator next = first;
  //将 remove_copy 的结果区间指定为 first
  return first == last ? first : remove_copy(++next, last, first, value);
}

template <class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
                          Predicate pred) {
  first = find_if(first, last, pred);
  ForwardIterator next = first;
  return first == last ? first : remove_copy_if(++next, last, first, pred);
}
// 将第一个序列中的"不重复元素(重复只复制一次)"复制到以result开始的区间中
template <class InputIterator, class ForwardIterator>
ForwardIterator __unique_copy(InputIterator first, InputIterator last,
                              ForwardIterator result, forward_iterator_tag) {
  *result = *first;
  while (++first != last)
    if (*result != *first) *++result = *first;
  return ++result;
}


template <class InputIterator, class OutputIterator, class T>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
                             OutputIterator result, T*) {
  T value = *first;
  *result = value;
  while (++first != last)
    if (value != *first) {
      value = *first;
      *++result = value;
    }
  return ++result;
}

template <class InputIterator, class OutputIterator>
inline OutputIterator __unique_copy(InputIterator first, InputIterator last,
                                    OutputIterator result, 
                                    output_iterator_tag) {
  return __unique_copy(first, last, result, value_type(first));
}

template <class InputIterator, class OutputIterator>
inline OutputIterator unique_copy(InputIterator first, InputIterator last,
                                  OutputIterator result) {
  if (first == last) return result;
  return __unique_copy(first, last, result, iterator_category(result));
}
template <class InputIterator, class ForwardIterator, class BinaryPredicate>
ForwardIterator __unique_copy(InputIterator first, InputIterator last,
                              ForwardIterator result, 
                              BinaryPredicate binary_pred,
                              forward_iterator_tag) {
  *result = *first;
  while (++first != last)
    if (!binary_pred(*result, *first)) *++result = *first;
  return ++result;
}

template <class InputIterator, class OutputIterator, class BinaryPredicate,
          class T>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
                             OutputIterator result,
                             BinaryPredicate binary_pred, T*) {
  T value = *first;
  *result = value;
  while (++first != last)
    if (!binary_pred(value, *first)) {
      value = *first;
      *++result = value;
    }
  return ++result;
}

template <class InputIterator, class OutputIterator, class BinaryPredicate>
inline OutputIterator __unique_copy(InputIterator first, InputIterator last,
                                    OutputIterator result,
                                    BinaryPredicate binary_pred,
                                    output_iterator_tag) {
  return __unique_copy(first, last, result, binary_pred, value_type(first));
}

template <class InputIterator, class OutputIterator, class BinaryPredicate>
inline OutputIterator unique_copy(InputIterator first, InputIterator last,
                                  OutputIterator result,
                                  BinaryPredicate binary_pred) {
  if (first == last) return result;
  return __unique_copy(first, last, result, binary_pred,
                       iterator_category(result));
}
//移除相邻的重复元素 (使用要先排序)
template <class ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
  first = adjacent_find(first, last);
  return unique_copy(first, last, first);
}

template <class ForwardIterator, class BinaryPredicate>
ForwardIterator unique(ForwardIterator first, ForwardIterator last,
                       BinaryPredicate binary_pred) {
  first = adjacent_find(first, last, binary_pred);
  return unique_copy(first, last, first, binary_pred);
}


/*
* 翻转当前区间,为了效率,设计成双层架构
* 对不同的迭代器调用不同的版本
*/
template <class RandomAccessIterator>
void __reverse(RandomAccessIterator first, RandomAccessIterator last,
               random_access_iterator_tag) {
  //只有 random iterator 才能做以下的 first < last 判断
  while (first < last) iter_swap(first++, --last);
}
template <class BidirectionalIterator>
void __reverse(BidirectionalIterator first, BidirectionalIterator last, 
               bidirectional_iterator_tag) {
  while (true)
    if (first == last || first == --last)
      return;
    else
      iter_swap(first++, last);
}

template <class BidirectionalIterator>
inline void reverse(BidirectionalIterator first, BidirectionalIterator last) {
  __reverse(first, last, iterator_category(first));
}

template <class BidirectionalIterator, class OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first,
                            BidirectionalIterator last,
                            OutputIterator result) {
  while (first != last) {
    --last;
    *result = *last;
    ++result;
  }
  return result;
}
/*
* 将 [first, middle) 和 [middle, last) 元素互换
* 这两个区间并不要求相等.
* 被设计成双层架构
*/
// 底层 ForwardIterator 版本 
template <class ForwardIterator, class Distance>
void __rotate(ForwardIterator first, ForwardIterator middle,
              ForwardIterator last, Distance*, forward_iterator_tag) {
  for (ForwardIterator i = middle; ;) {
    iter_swap(first, i); //交换前一段,后一段一一交换
    ++first;
    ++i;
    // 对于先结束的区间进行处理
    if (first == middle) {
      if (i == last) return;
      middle = i;
    }
    else if (i == last)
      i = middle;
  }
}

//底层 BidirectionalIterator 版本
template <class BidirectionalIterator, class Distance>
void __rotate(BidirectionalIterator first, BidirectionalIterator middle,
              BidirectionalIterator last, Distance*,
              bidirectional_iterator_tag) {
  reverse(first, middle);
  reverse(middle, last);
  reverse(first, last);
}
// 最大公因子,辗转相除法
template <class EuclideanRingElement>
EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n)
{
  while (n != 0) {
    EuclideanRingElement t = m % n;
    m = n;
    n = t;
  }
  return m;
}

template <class RandomAccessIterator, class Distance, class T>
void __rotate_cycle(RandomAccessIterator first, RandomAccessIterator last,
                    RandomAccessIterator initial, Distance shift, T*) {
  T value = *initial;
  RandomAccessIterator ptr1 = initial;
  RandomAccessIterator ptr2 = ptr1 + shift;
  while (ptr2 != initial) {
    *ptr1 = *ptr2;
    ptr1 = ptr2;
    if (last - ptr2 > shift)
      ptr2 += shift;
    else
      ptr2 = first + (shift - (last - ptr2));
  }
  *ptr1 = value;
}
// 底层 RandomAccessIterator 版本
template <class RandomAccessIterator, class Distance>
void __rotate(RandomAccessIterator first, RandomAccessIterator middle,
              RandomAccessIterator last, Distance*,
              random_access_iterator_tag) {
  Distance n = __gcd(last - first, middle - first);
  while (n--)
    __rotate_cycle(first, last, first + n, middle - first,
                   value_type(first));
}

template <class ForwardIterator>
inline void rotate(ForwardIterator first, ForwardIterator middle,
                   ForwardIterator last) {
  if (first == middle || middle == last) return;
  __rotate(first, middle, last, distance_type(first),
           iterator_category(first));
}


template <class ForwardIterator, class OutputIterator>
OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle,
                           ForwardIterator last, OutputIterator result) {
  return copy(first, middle, copy(middle, last, result));
}
// 将区间内的元素随机重新排列
template <class RandomAccessIterator, class Distance>
void __random_shuffle(RandomAccessIterator first, RandomAccessIterator last,
                      Distance*) {
  if (first == last) return;
  for (RandomAccessIterator i = first + 1; i != last; ++i) 
#ifdef __STL_NO_DRAND48
    iter_swap(i, first + Distance(rand() % ((i - first) + 1)));
#else
  iter_swap(i, first + Distance(lrand48() % ((i - first) + 1)));
#endif
}

template <class RandomAccessIterator>
inline void random_shuffle(RandomAccessIterator first,
                           RandomAccessIterator last) {
  __random_shuffle(first, last, distance_type(first));
}

template <class RandomAccessIterator, class RandomNumberGenerator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last,
                    RandomNumberGenerator& rand) {
  // 通过引用的方式传递,因此不同调用产生的随机数不同
  if (first == last) return;
  for (RandomAccessIterator i = first + 1; i != last; ++i)
    iter_swap(i, first + rand((i - first) + 1));
}

template <class ForwardIterator, class OutputIterator, class Distance>
OutputIterator random_sample_n(ForwardIterator first, ForwardIterator last,
                               OutputIterator out, const Distance n)
{
  Distance remaining = 0;
  distance(first, last, remaining);
  Distance m = min(n, remaining);

  while (m > 0) {
#ifdef __STL_NO_DRAND48
    if (rand() % remaining < m) {
#else
    if (lrand48() % remaining < m) {
#endif
      *out = *first;
      ++out;
      --m;
    }

    --remaining;
    ++first;
  }
  return out;
}

template <class ForwardIterator, class OutputIterator, class Distance,
          class RandomNumberGenerator>
OutputIterator random_sample_n(ForwardIterator first, ForwardIterator last,
                               OutputIterator out, const Distance n,
                               RandomNumberGenerator& rand)
{
  Distance remaining = 0;
  distance(first, last, remaining);
  Distance m = min(n, remaining);

  while (m > 0) {
    if (rand(remaining) < m) {
      *out = *first;
      ++out;
      --m;
    }

    --remaining;
    ++first;
  }
  return out;
}

template <class InputIterator, class RandomAccessIterator, class Distance>
RandomAccessIterator __random_sample(InputIterator first, InputIterator last,
                                     RandomAccessIterator out,
                                     const Distance n)
{
  Distance m = 0;
  Distance t = n;
  for ( ; first != last && m < n; ++m, ++first) 
    out[m] = *first;

  while (first != last) {
    ++t;
#ifdef __STL_NO_DRAND48
    Distance M = rand() % t;
#else
    Distance M = lrand48() % t;
#endif
    if (M < n)
      out[M] = *first;
    ++first;
  }

  return out + m;
}

template <class InputIterator, class RandomAccessIterator,
          class RandomNumberGenerator, class Distance>
RandomAccessIterator __random_sample(InputIterator first, InputIterator last,
                                     RandomAccessIterator out,
                                     RandomNumberGenerator& rand,
                                     const Distance n)
{
  Distance m = 0;
  Distance t = n;
  for ( ; first != last && m < n; ++m, ++first)
    out[m] = *first;

  while (first != last) {
    ++t;
    Distance M = rand(t);
    if (M < n)
      out[M] = *first;
    ++first;
  }

  return out + m;
}

template <class InputIterator, class RandomAccessIterator>
inline RandomAccessIterator
random_sample(InputIterator first, InputIterator last,
              RandomAccessIterator out_first, RandomAccessIterator out_last) 
{
  return __random_sample(first, last, out_first, out_last - out_first);
}

template <class InputIterator, class RandomAccessIterator, 
          class RandomNumberGenerator>
inline RandomAccessIterator
random_sample(InputIterator first, InputIterator last,
              RandomAccessIterator out_first, RandomAccessIterator out_last,
              RandomNumberGenerator& rand) 
{
  return __random_sample(first, last, out_first, rand, out_last - out_first);
}


/*
 *  将区间[first, last) 中的元素重新排列(按照 Pred 指定的规则,符合的元素放在区间前端,符合的元素
 *  放在区间后端)
 *  使用双指针算法，first 从前往后,last 从后往前,两者在遇见不符合条件的元素时,停止移动,
 *  交换 first 和 last 指向的元素，继续移动
 */
template <class BidirectionalIterator, class Predicate>
BidirectionalIterator partition(BidirectionalIterator first,
                                BidirectionalIterator last, Predicate pred) {
  while (true) {
    while (true) // 向后移动 first， 找到符合条件的元素
      if (first == last)
        return first;
      else if (pred(*first)) //符合条件 pred
        ++first;
      else
        break;

    --last;

    while (true) //向前移动 last, 找到符合条件的元素
      if (first == last)
        return first;
      else if (!pred(*last)) //不符合条件pred
        --last;
      else
        break;
    iter_swap(first, last); //交换 first 和last 指向的元素
    ++first;
  }
}

template <class ForwardIterator, class Predicate, class Distance>
ForwardIterator __inplace_stable_partition(ForwardIterator first,
                                           ForwardIterator last,
                                           Predicate pred, Distance len) {
  if (len == 1) return pred(*first) ? last : first;
  ForwardIterator middle = first;
  advance(middle, len / 2);
  ForwardIterator 
    first_cut = __inplace_stable_partition(first, middle, pred, len / 2);
  ForwardIterator 
    second_cut = __inplace_stable_partition(middle, last, pred,
                                            len - len / 2);
  rotate(first_cut, middle, second_cut);
  len = 0;
  distance(middle, second_cut, len);
  advance(first_cut, len);
  return first_cut;
}

template <class ForwardIterator, class Pointer, class Predicate, 
          class Distance>
ForwardIterator __stable_partition_adaptive(ForwardIterator first,
                                            ForwardIterator last,
                                            Predicate pred, Distance len,
                                            Pointer buffer,
                                            Distance buffer_size) {
  if (len <= buffer_size) {
    ForwardIterator result1 = first;
    Pointer result2 = buffer;
    for ( ; first != last ; ++first)
      if (pred(*first)) {
        *result1 = *first;
        ++result1;
      }
      else {
        *result2 = *first;
        ++result2;
      }
    copy(buffer, result2, result1);
    return result1;
  }
  else {
    ForwardIterator middle = first;
    advance(middle, len / 2);
    ForwardIterator first_cut =
      __stable_partition_adaptive(first, middle, pred, len / 2,
                                  buffer, buffer_size);
    ForwardIterator second_cut =
      __stable_partition_adaptive(middle, last, pred, len - len / 2,
                                  buffer, buffer_size);

    rotate(first_cut, middle, second_cut);
    len = 0;
    distance(middle, second_cut, len);
    advance(first_cut, len);
    return first_cut;
  }
}

template <class ForwardIterator, class Predicate, class T, class Distance>
inline ForwardIterator __stable_partition_aux(ForwardIterator first,
                                              ForwardIterator last, 
                                              Predicate pred, T*, Distance*) {
  temporary_buffer<ForwardIterator, T> buf(first, last);
  if (buf.size() > 0)
    return __stable_partition_adaptive(first, last, pred,
                                       Distance(buf.requested_size()),
                                       buf.begin(), buf.size());
  else
    return __inplace_stable_partition(first, last, pred, 
                                      Distance(buf.requested_size()));
}

template <class ForwardIterator, class Predicate>
inline ForwardIterator stable_partition(ForwardIterator first,
                                        ForwardIterator last, 
                                        Predicate pred) {
  if (first == last)
    return first;
  else
    return __stable_partition_aux(first, last, pred,
                                  value_type(first), distance_type(first));
}
//  分割,快排中分割算法
template <class RandomAccessIterator, class T>
RandomAccessIterator __unguarded_partition(RandomAccessIterator first, 
                                           RandomAccessIterator last, 
                                           T pivot) {
  while (true) {
    while (*first < pivot) ++first;
    --last;
    while (pivot < *last) --last;
    if (!(first < last)) return first;
    iter_swap(first, last);
    ++first;
  }
}    

template <class RandomAccessIterator, class T, class Compare>
RandomAccessIterator __unguarded_partition(RandomAccessIterator first, 
                                           RandomAccessIterator last, 
                                           T pivot, Compare comp) {
  while (1) {
    while (comp(*first, pivot)) ++first;
    --last;
    while (comp(pivot, *last)) --last;
    if (!(first < last)) return first;
    iter_swap(first, last);
    ++first;
  }
}

const int __stl_threshold = 16;

/*
* while 循环没有对于边界条件的判断,(所以命名为unguarded)
* 底层代码,一次比较的省略,带来的可能是效率的大幅提升
*/

template <class RandomAccessIterator, class T>
void __unguarded_linear_insert(RandomAccessIterator last, T value) {
  RandomAccessIterator next = last;
  --next;
  while (value < *next) { 
    *last = *next;
    last = next;
    --next; //左移动一个位置
  }
  *last = value;
}

template <class RandomAccessIterator, class T, class Compare>
void __unguarded_linear_insert(RandomAccessIterator last, T value, 
                               Compare comp) {
  RandomAccessIterator next = last;
  --next;  
  while (comp(value , *next)) {
    *last = *next;
    last = next;
    --next; 
  }
  *last = value;
}

template <class RandomAccessIterator, class T>
inline void __linear_insert(RandomAccessIterator first, 
                            RandomAccessIterator last, T*) {
  T value = *last;
  if (value < *first) {
    copy_backward(first, last, last + 1);//将整个区间向右移动一格
    *first = value;
  }
  else
    __unguarded_linear_insert(last, value);
}

template <class RandomAccessIterator, class T, class Compare>
inline void __linear_insert(RandomAccessIterator first, 
                            RandomAccessIterator last, T*, Compare comp) {
  T value = *last;
  if (comp(value, *first)) {
    copy_backward(first, last, last + 1);
    *first = value;
  }
  else
    __unguarded_linear_insert(last, value, comp);
}
// 借助 __linear_insert 实现插入排序
template <class RandomAccessIterator>
void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
  if (first == last) return; 
  for (RandomAccessIterator i = first + 1; i != last; ++i)
    // 在[first, i)形成一个子区间
    __linear_insert(first, i, value_type(first));
}

template <class RandomAccessIterator, class Compare>
void __insertion_sort(RandomAccessIterator first,
                      RandomAccessIterator last, Compare comp) {
  if (first == last) return;
  for (RandomAccessIterator i = first + 1; i != last; ++i)
    __linear_insert(first, i, value_type(first), comp);
}

template <class RandomAccessIterator, class T>
void __unguarded_insertion_sort_aux(RandomAccessIterator first, 
                                    RandomAccessIterator last, T*) {
  for (RandomAccessIterator i = first; i != last; ++i)
    __unguarded_linear_insert(i, T(*i));
}

template <class RandomAccessIterator>
inline void __unguarded_insertion_sort(RandomAccessIterator first, 
                                RandomAccessIterator last) {
  __unguarded_insertion_sort_aux(first, last, value_type(first));
}

template <class RandomAccessIterator, class T, class Compare>
void __unguarded_insertion_sort_aux(RandomAccessIterator first, 
                                    RandomAccessIterator last,
                                    T*, Compare comp) {
  for (RandomAccessIterator i = first; i != last; ++i)
    __unguarded_linear_insert(i, T(*i), comp);
}

template <class RandomAccessIterator, class Compare>
inline void __unguarded_insertion_sort(RandomAccessIterator first, 
                                       RandomAccessIterator last,
                                       Compare comp) {
  __unguarded_insertion_sort_aux(first, last, value_type(first), comp);
}
// 对 元素个数 < 16的子区间排序
template <class RandomAccessIterator>
void __final_insertion_sort(RandomAccessIterator first, 
                            RandomAccessIterator last) {
  if (last - first > __stl_threshold) {
    __insertion_sort(first, first + __stl_threshold);
    __unguarded_insertion_sort(first + __stl_threshold, last);
  }
  else
     //插入排序
    __insertion_sort(first, last);
}

template <class RandomAccessIterator, class Compare>
void __final_insertion_sort(RandomAccessIterator first, 
                            RandomAccessIterator last, Compare comp) {
  if (last - first > __stl_threshold) {
    __insertion_sort(first, first + __stl_threshold, comp);
    __unguarded_insertion_sort(first + __stl_threshold, last, comp);
  }
  else
    __insertion_sort(first, last, comp);
}
// 用来控制分割恶化的情况,找出 2^k <= n的最大值 k
template <class Size>
inline Size __lg(Size n) {
  Size k;
  for (k = 0; n > 1; n >>= 1) ++k;
  return k;
}

template <class RandomAccessIterator, class T, class Size>
void __introsort_loop(RandomAccessIterator first,
                      RandomAccessIterator last, T*,
                      Size depth_limit) {
  while (last - first > __stl_threshold) { // > 16
    if (depth_limit == 0) { //分割恶化改用 heapsort
      partial_sort(first, last, last);
      return;
    }
    --depth_limit;
    RandomAccessIterator cut = __unguarded_partition
      (first, last, T(__median(*first, *(first + (last - first)/2),
                               *(last - 1))));
    __introsort_loop(cut, last, value_type(first), depth_limit);
    last = cut;
  }
}

template <class RandomAccessIterator, class T, class Size, class Compare>
void __introsort_loop(RandomAccessIterator first,
                      RandomAccessIterator last, T*,
                      Size depth_limit, Compare comp) {
  while (last - first > __stl_threshold) {
    if (depth_limit == 0) {
      partial_sort(first, last, last, comp);
      return;
    }
    --depth_limit;
    RandomAccessIterator cut = __unguarded_partition
      (first, last, T(__median(*first, *(first + (last - first)/2),
                               *(last - 1), comp)), comp);
    __introsort_loop(cut, last, value_type(first), depth_limit, comp);
    last = cut;
  }
}
/*
* 利用快排和插入排序进行排序,先使用快排,当子区间足够小时使用插入排序
* 关联容器没有这个函数,因为自身有序
*/
template <class RandomAccessIterator>
inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
  if (first != last) {
    // 分割恶化就使用 heapsort (partial_sort)
    __introsort_loop(first, last, value_type(first), __lg(last - first) * 2);
    //对于元素个数 < 16的子序列使用
    __final_insertion_sort(first, last);
  }
}

template <class RandomAccessIterator, class Compare>
inline void sort(RandomAccessIterator first, RandomAccessIterator last,
                 Compare comp) {
  if (first != last) {
    __introsort_loop(first, last, value_type(first), __lg(last - first) * 2,
                     comp);
    __final_insertion_sort(first, last, comp);
  }
}


template <class RandomAccessIterator>
void __inplace_stable_sort(RandomAccessIterator first,
                           RandomAccessIterator last) {
  if (last - first < 15) {
    __insertion_sort(first, last);
    return;
  }
  RandomAccessIterator middle = first + (last - first) / 2;
  __inplace_stable_sort(first, middle);
  __inplace_stable_sort(middle, last);
  __merge_without_buffer(first, middle, last, middle - first, last - middle);
}

template <class RandomAccessIterator, class Compare>
void __inplace_stable_sort(RandomAccessIterator first,
                           RandomAccessIterator last, Compare comp) {
  if (last - first < 15) {
    __insertion_sort(first, last, comp);
    return;
  }
  RandomAccessIterator middle = first + (last - first) / 2;
  __inplace_stable_sort(first, middle, comp);
  __inplace_stable_sort(middle, last, comp);
  __merge_without_buffer(first, middle, last, middle - first,
                         last - middle, comp);
}

template <class RandomAccessIterator1, class RandomAccessIterator2,
          class Distance>
void __merge_sort_loop(RandomAccessIterator1 first,
                       RandomAccessIterator1 last, 
                       RandomAccessIterator2 result, Distance step_size) {
  Distance two_step = 2 * step_size;

  while (last - first >= two_step) {
    result = merge(first, first + step_size,
                   first + step_size, first + two_step, result);
    first += two_step;
  }

  step_size = min(Distance(last - first), step_size);
  merge(first, first + step_size, first + step_size, last, result);
}

template <class RandomAccessIterator1, class RandomAccessIterator2,
          class Distance, class Compare>
void __merge_sort_loop(RandomAccessIterator1 first,
                       RandomAccessIterator1 last, 
                       RandomAccessIterator2 result, Distance step_size,
                       Compare comp) {
  Distance two_step = 2 * step_size;

  while (last - first >= two_step) {
    result = merge(first, first + step_size,
                   first + step_size, first + two_step, result, comp);
    first += two_step;
  }
  step_size = min(Distance(last - first), step_size);

  merge(first, first + step_size, first + step_size, last, result, comp);
}

const int __stl_chunk_size = 7;
        
template <class RandomAccessIterator, class Distance>
void __chunk_insertion_sort(RandomAccessIterator first, 
                            RandomAccessIterator last, Distance chunk_size) {
  while (last - first >= chunk_size) {
    __insertion_sort(first, first + chunk_size);
    first += chunk_size;
  }
  __insertion_sort(first, last);
}

template <class RandomAccessIterator, class Distance, class Compare>
void __chunk_insertion_sort(RandomAccessIterator first, 
                            RandomAccessIterator last,
                            Distance chunk_size, Compare comp) {
  while (last - first >= chunk_size) {
    __insertion_sort(first, first + chunk_size, comp);
    first += chunk_size;
  }
  __insertion_sort(first, last, comp);
}

template <class RandomAccessIterator, class Pointer, class Distance>
void __merge_sort_with_buffer(RandomAccessIterator first, 
                              RandomAccessIterator last,
                              Pointer buffer, Distance*) {
  Distance len = last - first;
  Pointer buffer_last = buffer + len;

  Distance step_size = __stl_chunk_size;
  __chunk_insertion_sort(first, last, step_size);

  while (step_size < len) {
    __merge_sort_loop(first, last, buffer, step_size);
    step_size *= 2;
    __merge_sort_loop(buffer, buffer_last, first, step_size);
    step_size *= 2;
  }
}

template <class RandomAccessIterator, class Pointer, class Distance,
          class Compare>
void __merge_sort_with_buffer(RandomAccessIterator first, 
                              RandomAccessIterator last, Pointer buffer,
                              Distance*, Compare comp) {
  Distance len = last - first;
  Pointer buffer_last = buffer + len;

  Distance step_size = __stl_chunk_size;
  __chunk_insertion_sort(first, last, step_size, comp);

  while (step_size < len) {
    __merge_sort_loop(first, last, buffer, step_size, comp);
    step_size *= 2;
    __merge_sort_loop(buffer, buffer_last, first, step_size, comp);
    step_size *= 2;
  }
}

template <class RandomAccessIterator, class Pointer, class Distance>
void __stable_sort_adaptive(RandomAccessIterator first, 
                            RandomAccessIterator last, Pointer buffer,
                            Distance buffer_size) {
  Distance len = (last - first + 1) / 2;
  RandomAccessIterator middle = first + len;
  if (len > buffer_size) {
    __stable_sort_adaptive(first, middle, buffer, buffer_size);
    __stable_sort_adaptive(middle, last, buffer, buffer_size);
  } else {
    __merge_sort_with_buffer(first, middle, buffer, (Distance*)0);
    __merge_sort_with_buffer(middle, last, buffer, (Distance*)0);
  }
  __merge_adaptive(first, middle, last, Distance(middle - first), 
                   Distance(last - middle), buffer, buffer_size);
}

template <class RandomAccessIterator, class Pointer, class Distance, 
          class Compare>
void __stable_sort_adaptive(RandomAccessIterator first, 
                            RandomAccessIterator last, Pointer buffer,
                            Distance buffer_size, Compare comp) {
  Distance len = (last - first + 1) / 2;
  RandomAccessIterator middle = first + len;
  if (len > buffer_size) {
    __stable_sort_adaptive(first, middle, buffer, buffer_size, 
                           comp);
    __stable_sort_adaptive(middle, last, buffer, buffer_size, 
                           comp);
  } else {
    __merge_sort_with_buffer(first, middle, buffer, (Distance*)0, comp);
    __merge_sort_with_buffer(middle, last, buffer, (Distance*)0, comp);
  }
  __merge_adaptive(first, middle, last, Distance(middle - first), 
                   Distance(last - middle), buffer, buffer_size,
                   comp);
}

template <class RandomAccessIterator, class T, class Distance>
inline void __stable_sort_aux(RandomAccessIterator first,
                              RandomAccessIterator last, T*, Distance*) {
  temporary_buffer<RandomAccessIterator, T> buf(first, last);
  if (buf.begin() == 0)
    __inplace_stable_sort(first, last);
  else 
    __stable_sort_adaptive(first, last, buf.begin(), Distance(buf.size()));
}

template <class RandomAccessIterator, class T, class Distance, class Compare>
inline void __stable_sort_aux(RandomAccessIterator first,
                              RandomAccessIterator last, T*, Distance*,
                              Compare comp) {
  temporary_buffer<RandomAccessIterator, T> buf(first, last);
  if (buf.begin() == 0)
    __inplace_stable_sort(first, last, comp);
  else 
    __stable_sort_adaptive(first, last, buf.begin(), Distance(buf.size()),
                           comp);
}

template <class RandomAccessIterator>
inline void stable_sort(RandomAccessIterator first,
                        RandomAccessIterator last) {
  __stable_sort_aux(first, last, value_type(first), distance_type(first));
}

template <class RandomAccessIterator, class Compare>
inline void stable_sort(RandomAccessIterator first,
                        RandomAccessIterator last, Compare comp) {
  __stable_sort_aux(first, last, value_type(first), distance_type(first), 
                    comp);
}
// 局部排序 [fist, middle -first)
template <class RandomAccessIterator, class T>
void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
                    RandomAccessIterator last, T*) {
  make_heap(first, middle);
  for (RandomAccessIterator i = middle; i < last; ++i)
    if (*i < *first) //只适用于 random iterator
      __pop_heap(first, middle, i, T(*i), distance_type(first));
  sort_heap(first, middle);
}

template <class RandomAccessIterator, class T, class Compare>
void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
                    RandomAccessIterator last, T*, Compare comp) {
  make_heap(first, middle, comp);
  for (RandomAccessIterator i = middle; i < last; ++i)
    if (comp(*i, *first))
      __pop_heap(first, middle, i, T(*i), comp, distance_type(first));
  sort_heap(first, middle, comp);
}

template <class RandomAccessIterator>
inline void partial_sort(RandomAccessIterator first,
                         RandomAccessIterator middle,
                         RandomAccessIterator last) {
  __partial_sort(first, middle, last, value_type(first));
}

template <class RandomAccessIterator, class Compare>
inline void partial_sort(RandomAccessIterator first,
                         RandomAccessIterator middle,
                         RandomAccessIterator last, Compare comp) {
  __partial_sort(first, middle, last, value_type(first), comp);
}
// 与 __partial_sort 相同,不过会将结果保存[result_first, result_last) 
template <class InputIterator, class RandomAccessIterator, class Distance,
          class T>
RandomAccessIterator __partial_sort_copy(InputIterator first,
                                         InputIterator last,
                                         RandomAccessIterator result_first,
                                         RandomAccessIterator result_last, 
                                         Distance*, T*) {
  if (result_first == result_last) return result_last;
  RandomAccessIterator result_real_last = result_first;
  while(first != last && result_real_last != result_last) {
    *result_real_last = *first;
    ++result_real_last;
    ++first;
  }
  make_heap(result_first, result_real_last);
  while (first != last) {
    if (*first < *result_first) 
      __adjust_heap(result_first, Distance(0),
                    Distance(result_real_last - result_first), T(*first));
    ++first;
  }
  sort_heap(result_first, result_real_last);
  return result_real_last;
}

template <class InputIterator, class RandomAccessIterator>
inline RandomAccessIterator
partial_sort_copy(InputIterator first, InputIterator last,
                  RandomAccessIterator result_first,
                  RandomAccessIterator result_last) {
  return __partial_sort_copy(first, last, result_first, result_last, 
                             distance_type(result_first), value_type(first));
}

template <class InputIterator, class RandomAccessIterator, class Compare,
          class Distance, class T>
RandomAccessIterator __partial_sort_copy(InputIterator first,
                                         InputIterator last,
                                         RandomAccessIterator result_first,
                                         RandomAccessIterator result_last,
                                         Compare comp, Distance*, T*) {
  if (result_first == result_last) return result_last;
  RandomAccessIterator result_real_last = result_first;
  while(first != last && result_real_last != result_last) {
    *result_real_last = *first;
    ++result_real_last;
    ++first;
  }
  make_heap(result_first, result_real_last, comp);
  while (first != last) {
    if (comp(*first, *result_first))
      __adjust_heap(result_first, Distance(0),
                    Distance(result_real_last - result_first), T(*first),
                    comp);
    ++first;
  }
  sort_heap(result_first, result_real_last, comp);
  return result_real_last;
}

template <class InputIterator, class RandomAccessIterator, class Compare>
inline RandomAccessIterator
partial_sort_copy(InputIterator first, InputIterator last,
                  RandomAccessIterator result_first,
                  RandomAccessIterator result_last, Compare comp) {
  return __partial_sort_copy(first, last, result_first, result_last, comp,
                             distance_type(result_first), value_type(first));
}

template <class RandomAccessIterator, class T>
void __nth_element(RandomAccessIterator first, RandomAccessIterator nth,
                   RandomAccessIterator last, T*) {
  while (last - first > 3) {
    RandomAccessIterator cut = __unguarded_partition
      (first, last, T(__median(*first, *(first + (last - first)/2),
                               *(last - 1))));
    if (cut <= nth)
      first = cut;
    else 
      last = cut;
  }
  __insertion_sort(first, last);
}

template <class RandomAccessIterator>
inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
                        RandomAccessIterator last) {
  __nth_element(first, nth, last, value_type(first));
}

template <class RandomAccessIterator, class T, class Compare>
void __nth_element(RandomAccessIterator first, RandomAccessIterator nth,
                   RandomAccessIterator last, T*, Compare comp) {
  while (last - first > 3) {
    RandomAccessIterator cut = __unguarded_partition
      (first, last, T(__median(*first, *(first + (last - first)/2), 
                               *(last - 1), comp)), comp);
    if (cut <= nth)
      first = cut;
    else 
      last = cut;
  }
  __insertion_sort(first, last, comp);
}

template <class RandomAccessIterator, class Compare>
inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
                 RandomAccessIterator last, Compare comp) {
  __nth_element(first, nth, last, value_type(first), comp);
}
/*
* 二分查找的一个版本,在有序区间内寻找元素
* 将各个版本的 __lower_bound 进行比较
*/
template <class ForwardIterator, class T, class Distance>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
                              const T& value, Distance*,
                              forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);
  // 思考 len = last -first 不行吗?
  Distance half;
  ForwardIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);
    if (*middle < value) {
      first = middle; 
      ++first; //first 指向 middle的下一个位置
      len = len - half - 1;
    }
    else
      len = half;
  }
  return first;
}

template <class ForwardIterator, class T, class Compare, class Distance>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
                              const T& value, Compare comp, Distance*,
                              forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);
  Distance half;
  ForwardIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);
    if (comp(*middle, value)) {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else
      len = half;
  }
  return first;
}
// RandomAccessIterator 版本的辅助函数
template <class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first,
                                   RandomAccessIterator last, const T& value,
                                   Distance*, random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (*middle < value) {
      first = middle + 1; //直接加一
      len = len - half - 1;
    }
    else
      len = half;
  }
  return first;
}
template <class RandomAccessIterator, class T, class Compare, class Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first,
                                   RandomAccessIterator last,
                                   const T& value, Compare comp, Distance*,
                                   random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (comp(*middle, value)) {
      first = middle + 1;
      len = len - half - 1;
    }
    else
      len = half;
  }
  return first;
}

template <class ForwardIterator, class T>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
                                   const T& value) {
  return __lower_bound(first, last, value, distance_type(first),
                       iterator_category(first));
}

template <class ForwardIterator, class T, class Compare>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
                                   const T& value, Compare comp) {
  return __lower_bound(first, last, value, comp, distance_type(first),
                       iterator_category(first));
}

/*
*  二分查找版本,找到可插入 value 的最后一个"合适"的位置
*/

// ForwardIterator 版本辅助函数
template <class ForwardIterator, class T, class Distance>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
                              const T& value, Distance*,
                              forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);
  Distance half;
  ForwardIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);
    if (value < *middle)
      len = half;
    else {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}
template <class ForwardIterator, class T, class Compare, class Distance>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
                              const T& value, Compare comp, Distance*,
                              forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);
  Distance half;
  ForwardIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);
    if (comp(value, *middle))
      len = half;
    else {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}
// RandomAccessIterator 版本辅助函数
template <class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __upper_bound(RandomAccessIterator first,
                                   RandomAccessIterator last, const T& value,
                                   Distance*, random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (value < *middle)
      len = half;
    else {
      first = middle + 1;
      len = len - half - 1;
    }
  }
  return first;
}

template <class RandomAccessIterator, class T, class Compare, class Distance>
RandomAccessIterator __upper_bound(RandomAccessIterator first,
                                   RandomAccessIterator last,
                                   const T& value, Compare comp, Distance*,
                                   random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (comp(value, *middle))
      len = half;
    else {
      first = middle + 1;
      len = len - half - 1;
    }
  }
  return first;
}

template <class ForwardIterator, class T>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
                                   const T& value) {
  return __upper_bound(first, last, value, distance_type(first),
                       iterator_category(first));
}

template <class ForwardIterator, class T, class Compare>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
                                   const T& value, Compare comp) {
  return __upper_bound(first, last, value, comp, distance_type(first),
                       iterator_category(first));
}
// 二分查找,返回 可插入 value 的区间
template <class ForwardIterator, class T, class Distance>
pair<ForwardIterator, ForwardIterator>
__equal_range(ForwardIterator first, ForwardIterator last, const T& value,
              Distance*, forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);
  Distance half;
  ForwardIterator middle, left, right;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);
    if (*middle < value) {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else if (value < *middle)
      len = half;
    else {
      left = lower_bound(first, middle, value);
      advance(first, len);
      right = upper_bound(++middle, first, value);
      return pair<ForwardIterator, ForwardIterator>(left, right);
    }
  }
  return pair<ForwardIterator, ForwardIterator>(first, first);
}

template <class ForwardIterator, class T, class Compare, class Distance>
pair<ForwardIterator, ForwardIterator>
__equal_range(ForwardIterator first, ForwardIterator last, const T& value,
              Compare comp, Distance*, forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);
  Distance half;
  ForwardIterator middle, left, right;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);
    if (comp(*middle, value)) {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else if (comp(value, *middle))
      len = half;
    else {
      left = lower_bound(first, middle, value, comp);
      advance(first, len);
      right = upper_bound(++middle, first, value, comp);
      return pair<ForwardIterator, ForwardIterator>(left, right);
    }
  }
  return pair<ForwardIterator, ForwardIterator>(first, first);
}     

template <class RandomAccessIterator, class T, class Distance>
pair<RandomAccessIterator, RandomAccessIterator>
__equal_range(RandomAccessIterator first, RandomAccessIterator last,
              const T& value, Distance*, random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle, left, right;

  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (*middle < value) {
      first = middle + 1;
      len = len - half - 1;
    }
    else if (value < *middle)
      len = half;
    else {
      left = lower_bound(first, middle, value);
      right = upper_bound(++middle, first + len, value);
      return pair<RandomAccessIterator, RandomAccessIterator>(left,
                                                              right);
    }
  }
  return pair<RandomAccessIterator, RandomAccessIterator>(first, first);
}

template <class RandomAccessIterator, class T, class Compare, class Distance>
pair<RandomAccessIterator, RandomAccessIterator>
__equal_range(RandomAccessIterator first, RandomAccessIterator last,
              const T& value, Compare comp, Distance*,
              random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle, left, right;

  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (comp(*middle, value)) {
      first = middle + 1;
      len = len - half - 1;
    }
    else if (comp(value, *middle))
      len = half;
    else {
      left = lower_bound(first, middle, value, comp);
      right = upper_bound(++middle, first + len, value, comp);
      return pair<RandomAccessIterator, RandomAccessIterator>(left,
                                                              right);
    }
  }
  return pair<RandomAccessIterator, RandomAccessIterator>(first, first);
}     

template <class ForwardIterator, class T>
inline pair<ForwardIterator, ForwardIterator>
equal_range(ForwardIterator first, ForwardIterator last, const T& value) {
  return __equal_range(first, last, value, distance_type(first),
                       iterator_category(first));
}

template <class ForwardIterator, class T, class Compare>
inline pair<ForwardIterator, ForwardIterator>
equal_range(ForwardIterator first, ForwardIterator last, const T& value,
            Compare comp) {
  return __equal_range(first, last, value, comp, distance_type(first),
                       iterator_category(first));
}    

//借助lower_bound实现二分查找
template <class ForwardIterator, class T>
bool binary_search(ForwardIterator first, ForwardIterator last,
                   const T& value) {
  ForwardIterator i = lower_bound(first, last, value);
  return i != last && !(value < *i);
}
template <class ForwardIterator, class T, class Compare>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value,
                   Compare comp) {
  ForwardIterator i = lower_bound(first, last, value, comp);
  return i != last && !comp(value, *i);
}
// 将两个有序区间，进行合并
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result) {
  while (first1 != last1 && first2 != last2) {
    if (*first2 < *first1) {
      *result = *first2;
      ++first2;
    }
    else {
      *result = *first1;
      ++first1;
    }
    ++result;
  }
  //copy 剩余元素
  return copy(first2, last2, copy(first1, last1, result));
}

template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result, Compare comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first2, *first1)) {
      *result = *first2;
      ++first2;
    }
    else {
      *result = *first1;
      ++first1;
    }
    ++result;
  }
  return copy(first2, last2, copy(first1, last1, result));
}

template <class BidirectionalIterator, class Distance>
void __merge_without_buffer(BidirectionalIterator first,
                            BidirectionalIterator middle,
                            BidirectionalIterator last,
                            Distance len1, Distance len2) {
  if (len1 == 0 || len2 == 0) return;
  if (len1 + len2 == 2) {
    if (*middle < *first) iter_swap(first, middle);
    return;
  }
  BidirectionalIterator first_cut = first;
  BidirectionalIterator second_cut = middle;
  Distance len11 = 0;
  Distance len22 = 0;
  if (len1 > len2) {
    len11 = len1 / 2;
    advance(first_cut, len11);
    second_cut = lower_bound(middle, last, *first_cut);
    distance(middle, second_cut, len22);
  }
  else {
    len22 = len2 / 2;
    advance(second_cut, len22);
    first_cut = upper_bound(first, middle, *second_cut);
    distance(first, first_cut, len11);
  }
  rotate(first_cut, middle, second_cut);
  BidirectionalIterator new_middle = first_cut;
  advance(new_middle, len22);
  __merge_without_buffer(first, first_cut, new_middle, len11, len22);
  __merge_without_buffer(new_middle, second_cut, last, len1 - len11,
                         len2 - len22);
}

template <class BidirectionalIterator, class Distance, class Compare>
void __merge_without_buffer(BidirectionalIterator first,
                            BidirectionalIterator middle,
                            BidirectionalIterator last,
                            Distance len1, Distance len2, Compare comp) {
  if (len1 == 0 || len2 == 0) return;
  if (len1 + len2 == 2) {
    if (comp(*middle, *first)) iter_swap(first, middle);
    return;
  }
  BidirectionalIterator first_cut = first;
  BidirectionalIterator second_cut = middle;
  Distance len11 = 0;
  Distance len22 = 0;
  if (len1 > len2) {
    len11 = len1 / 2;
    advance(first_cut, len11);
    second_cut = lower_bound(middle, last, *first_cut, comp);
    distance(middle, second_cut, len22);
  }
  else {
    len22 = len2 / 2;
    advance(second_cut, len22);
    first_cut = upper_bound(first, middle, *second_cut, comp);
    distance(first, first_cut, len11);
  }
  rotate(first_cut, middle, second_cut);
  BidirectionalIterator new_middle = first_cut;
  advance(new_middle, len22);
  __merge_without_buffer(first, first_cut, new_middle, len11, len22, comp);
  __merge_without_buffer(new_middle, second_cut, last, len1 - len11,
                         len2 - len22, comp);
}

template <class BidirectionalIterator1, class BidirectionalIterator2,
          class Distance>
BidirectionalIterator1 __rotate_adaptive(BidirectionalIterator1 first,
                                         BidirectionalIterator1 middle,
                                         BidirectionalIterator1 last,
                                         Distance len1, Distance len2,
                                         BidirectionalIterator2 buffer,
                                         Distance buffer_size) {
  BidirectionalIterator2 buffer_end;
  if (len1 > len2 && len2 <= buffer_size) {
    buffer_end = copy(middle, last, buffer);
    copy_backward(first, middle, last);
    return copy(buffer, buffer_end, first);
  } else if (len1 <= buffer_size) {
    buffer_end = copy(first, middle, buffer);
    copy(middle, last, first);
    return copy_backward(buffer, buffer_end, last);
  } else  {
    rotate(first, middle, last);
    advance(first, len2);
    return first;
  }
}

template <class BidirectionalIterator1, class BidirectionalIterator2,
          class BidirectionalIterator3>
BidirectionalIterator3 __merge_backward(BidirectionalIterator1 first1,
                                        BidirectionalIterator1 last1,
                                        BidirectionalIterator2 first2,
                                        BidirectionalIterator2 last2,
                                        BidirectionalIterator3 result) {
  if (first1 == last1) return copy_backward(first2, last2, result);
  if (first2 == last2) return copy_backward(first1, last1, result);
  --last1;
  --last2;
  while (true) {
    if (*last2 < *last1) {
      *--result = *last1;
      if (first1 == last1) return copy_backward(first2, ++last2, result);
      --last1;
    }
    else {
      *--result = *last2;
      if (first2 == last2) return copy_backward(first1, ++last1, result);
      --last2;
    }
  }
}

template <class BidirectionalIterator1, class BidirectionalIterator2,
          class BidirectionalIterator3, class Compare>
BidirectionalIterator3 __merge_backward(BidirectionalIterator1 first1,
                                        BidirectionalIterator1 last1,
                                        BidirectionalIterator2 first2,
                                        BidirectionalIterator2 last2,
                                        BidirectionalIterator3 result,
                                        Compare comp) {
  if (first1 == last1) return copy_backward(first2, last2, result);
  if (first2 == last2) return copy_backward(first1, last1, result);
  --last1;
  --last2;
  while (true) {
    if (comp(*last2, *last1)) {
      *--result = *last1;
      if (first1 == last1) return copy_backward(first2, ++last2, result);
      --last1;
    }
    else {
      *--result = *last2;
      if (first2 == last2) return copy_backward(first1, ++last1, result);
      --last2;
    }
  }
}

template <class BidirectionalIterator, class Distance, class Pointer>
void __merge_adaptive(BidirectionalIterator first, 
                      BidirectionalIterator middle, 
                      BidirectionalIterator last, Distance len1, Distance len2,
                      Pointer buffer, Distance buffer_size) {
  if (len1 <= len2 && len1 <= buffer_size) {
    Pointer end_buffer = copy(first, middle, buffer);
    merge(buffer, end_buffer, middle, last, first);
  }
  else if (len2 <= buffer_size) {
    Pointer end_buffer = copy(middle, last, buffer);
    __merge_backward(first, middle, buffer, end_buffer, last);
  }
  else {
    BidirectionalIterator first_cut = first;
    BidirectionalIterator second_cut = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if (len1 > len2) {
      len11 = len1 / 2;
      advance(first_cut, len11);
      second_cut = lower_bound(middle, last, *first_cut);
      distance(middle, second_cut, len22);   
    }
    else {
      len22 = len2 / 2;
      advance(second_cut, len22);
      first_cut = upper_bound(first, middle, *second_cut);
      distance(first, first_cut, len11);
    }
    BidirectionalIterator new_middle =
      __rotate_adaptive(first_cut, middle, second_cut, len1 - len11,
                        len22, buffer, buffer_size);
    __merge_adaptive(first, first_cut, new_middle, len11, len22, buffer,
                     buffer_size);
    __merge_adaptive(new_middle, second_cut, last, len1 - len11,
                     len2 - len22, buffer, buffer_size);
  }
}

template <class BidirectionalIterator, class Distance, class Pointer,
          class Compare>
void __merge_adaptive(BidirectionalIterator first, 
                      BidirectionalIterator middle, 
                      BidirectionalIterator last, Distance len1, Distance len2,
                      Pointer buffer, Distance buffer_size, Compare comp) {
  if (len1 <= len2 && len1 <= buffer_size) {
    Pointer end_buffer = copy(first, middle, buffer);
    merge(buffer, end_buffer, middle, last, first, comp);
  }
  else if (len2 <= buffer_size) {
    Pointer end_buffer = copy(middle, last, buffer);
    __merge_backward(first, middle, buffer, end_buffer, last, comp);
  }
  else {
    BidirectionalIterator first_cut = first;
    BidirectionalIterator second_cut = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if (len1 > len2) {
      len11 = len1 / 2;
      advance(first_cut, len11);
      second_cut = lower_bound(middle, last, *first_cut, comp);
      distance(middle, second_cut, len22);   
    }
    else {
      len22 = len2 / 2;
      advance(second_cut, len22);
      first_cut = upper_bound(first, middle, *second_cut, comp);
      distance(first, first_cut, len11);
    }
    BidirectionalIterator new_middle =
      __rotate_adaptive(first_cut, middle, second_cut, len1 - len11,
                        len22, buffer, buffer_size);
    __merge_adaptive(first, first_cut, new_middle, len11, len22, buffer,
                     buffer_size, comp);
    __merge_adaptive(new_middle, second_cut, last, len1 - len11,
                     len2 - len22, buffer, buffer_size, comp);
  }
}

template <class BidirectionalIterator, class T, class Distance>
inline void __inplace_merge_aux(BidirectionalIterator first,
                                BidirectionalIterator middle,
                                BidirectionalIterator last, T*, Distance*) 
                                {

  Distance len1 = 0;
  distance(first, middle, len1);  // 序列一的长度
  Distance len2 = 0;
  distance(middle, last, len2); // 序列二的长度

  temporary_buffer<BidirectionalIterator, T> buf(first, last);
  if (buf.begin() == 0)
    __merge_without_buffer(first, middle, last, len1, len2);
  else
    __merge_adaptive(first, middle, last, len1, len2,
                     buf.begin(), Distance(buf.size()));
}

template <class BidirectionalIterator, class T, class Distance, class Compare>
inline void __inplace_merge_aux(BidirectionalIterator first,
                                BidirectionalIterator middle,
                                BidirectionalIterator last, T*, Distance*,
                                Compare comp) {
  Distance len1 = 0;
  distance(first, middle, len1);
  Distance len2 = 0;
  distance(middle, last, len2);

  temporary_buffer<BidirectionalIterator, T> buf(first, last);
  if (buf.begin() == 0)
    __merge_without_buffer(first, middle, last, len1, len2, comp);
  else
    __merge_adaptive(first, middle, last, len1, len2,
                     buf.begin(), Distance(buf.size()),
                     comp);
}
// 合并有序区间,使得合并后任然有序
template <class BidirectionalIterator>
inline void inplace_merge(BidirectionalIterator first,
                          BidirectionalIterator middle,
                          BidirectionalIterator last) {
  if (first == middle || middle == last) return;
  __inplace_merge_aux(first, middle, last, value_type(first),
                      distance_type(first));
}

template <class BidirectionalIterator, class Compare>
inline void inplace_merge(BidirectionalIterator first,
                          BidirectionalIterator middle,
                          BidirectionalIterator last, Compare comp) {
  if (first == middle || middle == last) return;
  __inplace_merge_aux(first, middle, last, value_type(first),
                      distance_type(first), comp);
}
// 判断序列2 是否"涵盖于" 序列1 (应用于有序区间)
template <class InputIterator1, class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2) {
  while (first1 != last1 && first2 != last2)
    if (*first2 < *first1)
      return false;
    else if(*first1 < *first2) 
      ++first1;
    else
      ++first1, ++first2;

  return first2 == last2;
}
/*
* comp 的"类型"是 Compare，既不是 BinaryPredicate 也不是 BinaryOperation
* 所以并不是任意的 二元操作符就可拿来作为 comp 参数
*/
template <class InputIterator1, class InputIterator2, class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2, Compare comp) {
  while (first1 != last1 && first2 != last2)
    if (comp(*first2, *first1))
      return false;
    else if(comp(*first1, *first2)) 
      ++first1;
    else //传入的  Compare 应该指定 case3 情况，否则 会导致语意错误
      ++first1, ++first2;

  return first2 == last2;
}
/*
*  算法 set_union 可构造 S1、S2 之并集
*  S1 和 S2 及其并集都是以排序区间表示的
*/
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result) {
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      *result = *first1;
      ++first1;
    }
    else if (*first2 < *first1) {
      *result = *first2;
      ++first2;
    }
    else {
      *result = *first1;
      ++first1;
      ++first2;
    }
    ++result;
  }
  //一段到达尾端，则将剩余元素拷贝到目的端
  return copy(first2, last2, copy(first1, last1, result));
}

template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result, Compare comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      *result = *first1;
      ++first1;
    }
    else if (comp(*first2, *first1)) {
      *result = *first2;
      ++first2;
    }
    else {
      *result = *first1;
      ++first1;
      ++first2;
    }
    ++result;
  }
  return copy(first2, last2, copy(first1, last1, result));
}
/*
*  可构造出 S1 、S2 的交集
*/
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result) {
  while (first1 != last1 && first2 != last2) 
    if (*first1 < *first2) 
      ++first1;
    else if (*first2 < *first1) 
      ++first2;
    else {
      *result = *first1;
      ++first1;
      ++first2;
      ++result;
    }
  return result;
}

template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result, Compare comp) {
  while (first1 != last1 && first2 != last2)
    if (comp(*first1, *first2))
      ++first1;
    else if (comp(*first2, *first1))
      ++first2;
    else {
      *result = *first1;
      ++first1;
      ++first2;
      ++result;
    }
  return result;
}
/*
*  构造出 S1、S2 的差集
*/
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              OutputIterator result) {
  while (first1 != last1 && first2 != last2)
    if (*first1 < *first2) {
      *result = *first1;
      ++first1;
      ++result;
    }
    else if (*first2 < *first1)
      ++first2;
    else {
      ++first1;
      ++first2;
    }
  return copy(first1, last1, result);
}

template <class InputIterator1, class InputIterator2, class OutputIterator, 
          class Compare>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2, 
                              OutputIterator result, Compare comp) {
  while (first1 != last1 && first2 != last2)
    if (comp(*first1, *first2)) {
      *result = *first1;
      ++first1;
      ++result;
    }
    else if (comp(*first2, *first1))
      ++first2;
    else {
      ++first1;
      ++first2;
    }
  return copy(first1, last1, result);
}
/*
 构造出对称差集 (S1 - S2) U (S2 - S1)
*/
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1,
                                        InputIterator1 last1,
                                        InputIterator2 first2,
                                        InputIterator2 last2,
                                        OutputIterator result) {
  while (first1 != last1 && first2 != last2)
    if (*first1 < *first2) {
      *result = *first1;
      ++first1;
      ++result;
    }
    else if (*first2 < *first1) {
      *result = *first2;
      ++first2;
      ++result;
    }
    else {
      ++first1;
      ++first2;
    }
  return copy(first2, last2, copy(first1, last1, result));
}

template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator set_symmetric_difference(InputIterator1 first1,
                                        InputIterator1 last1,
                                        InputIterator2 first2,
                                        InputIterator2 last2,
                                        OutputIterator result, Compare comp) {
  while (first1 != last1 && first2 != last2)
    if (comp(*first1, *first2)) {
      *result = *first1;
      ++first1;
      ++result;
    }
    else if (comp(*first2, *first1)) {
      *result = *first2;
      ++first2;
      ++result;
    }
    else {
      ++first1;
      ++first2;
    }
  return copy(first2, last2, copy(first1, last1, result));
}
// 返回最大值
template <class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last) 
    if (*result < *first) result = first;
  return result;
}

template <class ForwardIterator, class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last,
                            Compare comp) {
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last) 
    if (comp(*result, *first)) result = first;
  return result;
}

template <class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last) 
    if (*first < *result) result = first;
  return result;
}

template <class ForwardIterator, class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last,
                            Compare comp) {
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last) 
    if (comp(*first, *result)) result = first;
  return result;
}
// 返回下一个排列组合的算法
template <class BidirectionalIterator>
bool next_permutation(BidirectionalIterator first,
                      BidirectionalIterator last) {
  if (first == last) return false;
  BidirectionalIterator i = first;
  ++i;
  if (i == last) return false;
  i = last;
  --i;

  for(;;) {
    BidirectionalIterator ii = i;
    --i;
    if (*i < *ii) {
      BidirectionalIterator j = last;
      while (!(*i < *--j));
      iter_swap(i, j);
      reverse(ii, last);
      return true;
    }
    if (i == first) {
      reverse(first, last);
      return false;
    }
  }
}

template <class BidirectionalIterator, class Compare>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last,
                      Compare comp) {
  if (first == last) return false;
  BidirectionalIterator i = first;
  ++i;
  if (i == last) return false;
  i = last;
  --i;

  for(;;) {
    BidirectionalIterator ii = i;
    --i;
    if (comp(*i, *ii)) {
      BidirectionalIterator j = last;
      while (!comp(*i, *--j));
      iter_swap(i, j);
      reverse(ii, last);
      return true;
    }
    if (i == first) {
      reverse(first, last);
      return false;
    }
  }
}
// 返回前一个排列组合的算法

template <class BidirectionalIterator>
bool prev_permutation(BidirectionalIterator first,
                      BidirectionalIterator last) {
  if (first == last) return false;
  BidirectionalIterator i = first;
  ++i;
  if (i == last) return false;
  i = last;
  --i;

  for(;;) {
    BidirectionalIterator ii = i;
    --i;
    if (*ii < *i) {
      BidirectionalIterator j = last;
      while (!(*--j < *i));
      iter_swap(i, j);
      reverse(ii, last);
      return true;
    }
    if (i == first) {
      reverse(first, last);
      return false;
    }
  }
}

template <class BidirectionalIterator, class Compare>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last,
                      Compare comp) {
  if (first == last) return false;
  BidirectionalIterator i = first;
  ++i;
  if (i == last) return false;
  i = last;
  --i;

  for(;;) {
    BidirectionalIterator ii = i;
    --i;
    if (comp(*ii, *i)) {
      BidirectionalIterator j = last;
      while (!comp(*--j, *i));
      iter_swap(i, j);
      reverse(ii, last);
      return true;
    }
    if (i == first) {
      reverse(first, last);
      return false;
    }
  }
}
/* 与 find_end 相反，这里查找[first1, last1) 区间内[first2, last2)某些元素第一次出现的
  例如 在 序列1 中查找元音元素中莫一个元素出现的位置 (序列2为 aeiou)
*/
template <class InputIterator, class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2)
{
  for ( ; first1 != last1; ++first1) //遍历序列1
    for (ForwardIterator iter = first2; iter != last2; ++iter)//遍历序列二
      if (*first1 == *iter) 
        return first1;
  return last1;
}

template <class InputIterator, class ForwardIterator, class BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2,
                            BinaryPredicate comp)
{
  for ( ; first1 != last1; ++first1) 
    for (ForwardIterator iter = first2; iter != last2; ++iter)
      if (comp(*first1, *iter))
        return first1;
  return last1;
}


// Search [first2, last2) as a subsequence in [first1, last1).
// find_end for forward iterators. 
template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2,
                            forward_iterator_tag, forward_iterator_tag)
{
  if (first2 == last2) //查找目标为空
    return last1;
  else {
    ForwardIterator1 result = last1;
    while (1) {
      //查找 某个子序列的首次出现点，找不到返回last1
      ForwardIterator1 new_result = search(first1, last1, first2, last2);
      if (new_result == last1) //没找到
        return result;
      else {
        result = new_result;//准备下一次查找
        first1 = new_result;
        ++first1;
      }
    }
  }
}

template <class ForwardIterator1, class ForwardIterator2,
          class BinaryPredicate>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2,
                            forward_iterator_tag, forward_iterator_tag,
                            BinaryPredicate comp)
{
  if (first2 == last2)
    return last1;
  else {
    ForwardIterator1 result = last1;
    while (1) {
      ForwardIterator1 new_result = search(first1, last1, first2, last2, comp);
      if (new_result == last1)
        return result;
      else {
        result = new_result;
        first1 = new_result; 
        ++first1;
      }
    }
  }
}

// find_end for bidirectional iterators.  Requires partial specialization.
#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
// BidirectionalIterator版本
template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator1
__find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
           BidirectionalIterator2 first2, BidirectionalIterator2 last2,
           bidirectional_iterator_tag, bidirectional_iterator_tag)
{
  //由于查询的是"最后出现地点"，所以反向查找较快
  typedef reverse_iterator<BidirectionalIterator1> reviter1;
  typedef reverse_iterator<BidirectionalIterator2> reviter2;

  reviter1 rlast1(first1);
  reviter2 rlast2(first2);
  reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2);

  if (rresult == rlast1)
    return last1;
  else {
    BidirectionalIterator1 result = rresult.base(); //转回正常的迭代器,此时指向的 序列2 的尾部
    advance(result, -distance(first2, last2));//转到序列开头的位置
    return result;
  }
}

template <class BidirectionalIterator1, class BidirectionalIterator2,
          class BinaryPredicate>
BidirectionalIterator1
__find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
           BidirectionalIterator2 first2, BidirectionalIterator2 last2,
           bidirectional_iterator_tag, bidirectional_iterator_tag, 
           BinaryPredicate comp)
{
  typedef reverse_iterator<BidirectionalIterator1> reviter1;
  typedef reverse_iterator<BidirectionalIterator2> reviter2;

  reviter1 rlast1(first1);
  reviter2 rlast2(first2);
  reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2,
                            comp);

  if (rresult == rlast1)
    return last1;
  else {
    BidirectionalIterator1 result = rresult.base();
    advance(result, -distance(first2, last2));
    return result;
  }
}
#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

// 分派函数 Dispatching functions.

template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 
find_end(ForwardIterator1 first1, ForwardIterator1 last1, 
         ForwardIterator2 first2, ForwardIterator2 last2)
{
#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
   
  typedef typename iterator_traits<ForwardIterator1>::iterator_category
          category1;
  typedef typename iterator_traits<ForwardIterator2>::iterator_category
          category2;
  // 根据 两个区间的迭代器类型，调用不同的底层函数
  return __find_end(first1, last1, first2, last2, category1(), category2());
#else /* __STL_CLASS_PARTIAL_SPECIALIZATION */
  return __find_end(first1, last1, first2, last2,
                    forward_iterator_tag(), forward_iterator_tag());
#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */
}

template <class ForwardIterator1, class ForwardIterator2, 
          class BinaryPredicate>
inline ForwardIterator1 
find_end(ForwardIterator1 first1, ForwardIterator1 last1, 
         ForwardIterator2 first2, ForwardIterator2 last2,
         BinaryPredicate comp)
{
#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
  typedef typename iterator_traits<ForwardIterator1>::iterator_category
          category1;
  typedef typename iterator_traits<ForwardIterator2>::iterator_category
          category2;
  return __find_end(first1, last1, first2, last2, category1(), category2(),
                    comp);
#else /* __STL_CLASS_PARTIAL_SPECIALIZATION */
  return __find_end(first1, last1, first2, last2,
                    forward_iterator_tag(), forward_iterator_tag(),
                    comp);
#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */
}

template <class RandomAccessIterator, class Distance>
bool __is_heap(RandomAccessIterator first, RandomAccessIterator last,
               Distance*)
{
  const Distance n = last - first;

  Distance parent = 0;
  for (Distance child = 1; child < n; ++child) {
    if (first[parent] < first[child]) 
      return false;
    if ((child & 1) == 0)
      ++parent;
  }
  return true;
}

template <class RandomAccessIterator>
inline bool is_heap(RandomAccessIterator first, RandomAccessIterator last)
{
  return __is_heap(first, last, distance_type(first));
}


template <class RandomAccessIterator, class Distance, class StrictWeakOrdering>
bool __is_heap(RandomAccessIterator first, RandomAccessIterator last,
               StrictWeakOrdering comp,
               Distance*)
{
  const Distance n = last - first;

  Distance parent = 0;
  for (Distance child = 1; child < n; ++child) {
    if (comp(first[parent], first[child]))
      return false;
    if ((child & 1) == 0)
      ++parent;
  }
  return true;
}

template <class RandomAccessIterator, class StrictWeakOrdering>
inline bool is_heap(RandomAccessIterator first, RandomAccessIterator last,
                    StrictWeakOrdering comp)
{
  return __is_heap(first, last, comp, distance_type(first));
}


template <class ForwardIterator>
bool is_sorted(ForwardIterator first, ForwardIterator last)
{
  if (first == last)
    return true;

  ForwardIterator next = first;
  for (++next; next != last; first = next, ++next) {
    if (*next < *first)
      return false;
  }

  return true;
}

template <class ForwardIterator, class StrictWeakOrdering>
bool is_sorted(ForwardIterator first, ForwardIterator last,
               StrictWeakOrdering comp)
{
  if (first == last)
    return true;

  ForwardIterator next = first;
  for (++next; next != last; first = next, ++next) {
    if (comp(*next, *first))
      return false;
  }

  return true;
}

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1209
#endif

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_ALGO_H */

// Local Variables:
// mode:C++
// End:
