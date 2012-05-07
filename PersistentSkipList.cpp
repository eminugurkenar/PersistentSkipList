///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PersistentSkipList.cpp                                           //
//                                                                           //
// MODULE:  Persistent Skip List                                             //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef PERSISTENTSKIPLIST_CPP
#define PERSISTENTSKIPLIST_CPP

#include "PersistentSkipList.hpp"

using namespace persistent_skip_list;

///////////////////////////////////////////////////////////////////////////////
// ListNode Implementation                                                   //
///////////////////////////////////////////////////////////////////////////////

template<class T>
bool ListNode<T>::_SEEDED = false;

template<class T>
void ListNode<T>::seed() {
      if(_SEEDED)
	return;
      _SEEDED = true;
      time_t seed = (time_t)1312564825;
      seed = time(0); // comment this for non-random seeding
      srand( seed );
      if(PSL_DEBUG_MODE) {
	clog << "Seeding with value " << seed << endl;
      }
}

template<class T>
ListNode<T>::ListNode(const T& original_data)
  : next(), data(original_data)
{
  seed();
  // pick height, modified from Pat Morin's Open Data Structures
  height = 1;
  int bitCheck = 1;
  int r = rand();
  // check each bit in the binary representation of r, from the
  // least significant to the most significant.  The number of 1's
  // in a row from the least significant position determines the
  // height of the node.
  while((r & bitCheck) != 0) {
    // if bit is 1, increment height
    ++height;
    // check next bit
    bitCheck <<= 1;
  }

  // We know that the number of incoming pointers will be at most height
  in_nodes = new ListNode<T>*[height];
  for(int i = 0; i < height; ++i)
    in_nodes[i] = NULL;
}

template<class T>
ListNode<T>::~ListNode() {
  // clean up next
  for(int i = 0; i < (int)next.size(); ++i) {
    TSA* tsa = next[i];
    if(tsa != NULL) {
      for(int j = 0; j < tsa->getSize(); ++j) {
	ListNode<T>* ln = NULL;
	ln = tsa->getElement(j);
      }
      delete tsa;      // delete timestamped array
    }
  }
  delete[] in_nodes;
}

template<class T>
ListNode<T>* ListNode<T>::create(const T& original_data) {
  return new ListNode<T>(original_data);
}

template<class T>
T ListNode<T>::getData() {
  assert(this != NULL);
  return data;
}

template<class T>
int ListNode<T>::getHeight() {
  assert(this != NULL);
  return height;
}
  
template<class T>
int ListNode<T>::getNextChangeIndex(int t) {
  assert(this != NULL);
  int index = -1;
  int begin = 0, end = numberOfNextChangeIndices() -1;
  int timeFound = 0;
  // binary search
  while(begin <= end) {
    index = (begin+end)/2;
    timeFound = getNextAtIndex(index)->getTime();
    if(timeFound == t) {
      // done, break out of loop
      break;
    } else if(timeFound > t) {
      // repeat binary search on left (earlier) half
      end = index -1;
    } else {
      // repeat binary search on right (later) half
      begin = index +1;
    }
  }
  // report final index
  return index;
}

template <class T>
TimeStampedArray<ListNode<T>*>* ListNode<T>::getNextAtIndex(int ci) {
  assert(this != NULL);
  assert(ci >= 0);
  assert(ci < numberOfNextChangeIndices());
  return next[ci];
}

template <class T>
int ListNode<T>::numberOfNextChangeIndices() {
  assert(this != NULL);
  return (int)next.size();
}
  
template <class T>
TimeStampedArray<ListNode<T>*>* ListNode<T>::getNext(int t) {
  assert(this != NULL);
  assert(t >= 0);
  // find nearest time
  int index = getNextChangeIndex(t);
  // if there are no pointers, bail
  if(index == -1) return NULL;
  // use this to store the node to be returned
  TSA* tsa = getNextAtIndex(index);
  // since the nearest time might be later than the desired time, we
  // go backwards in time until we reach the point which immediately
  // precedes the query time
  while(tsa->getTime() > t) {
    --index;
    if(index < 0)
      return NULL;
    tsa = getNextAtIndex(index);
  }
  return tsa;
}
  
template <class T>
ListNode<T>* ListNode<T>::getNext(int t, int h) {
  assert(this != NULL);
  assert(t >= 0);
  TSA* tsa = getNext(t);
  if(tsa == NULL)
    return NULL;
  assert(h < tsa->getSize());
  return tsa->getElement(h);
}

template <class T>
int ListNode<T>::addNext(TimeStampedArray<ListNode<T>*>* tsa) {
  assert(this != NULL);
  // since NULL is the default
  if(tsa == NULL)
    return -1; // bail if trying to set next to NULL
  // make sure time is strictly increasing
  int lastIndex = (int)next.size()-1;
  if(lastIndex >= 0)
    assert(tsa->getTime() > next[lastIndex]->getTime());
  // finally, save the new set of next pointers
  next.push_back(tsa);
  // success
  return 0;
}

template <class T>
int ListNode<T>::addIncomingNode(int h, ListNode<T>* in) {
  assert(this != NULL);
  assert(h >= 0);
  assert(h < height);
  assert(in_nodes[h] == NULL);
  in_nodes[h] = in;
  return 0;
}

template <class T>
ListNode<T>* ListNode<T>::getIncomingNode(int h) {
  assert(this != NULL);
  assert(h >= 0);
  assert(h < height);
  return in_nodes[h];
}

template <class T>
int ListNode<T>::removeIncomingNode(int h) {
  assert(this != NULL);
  assert(h >= 0);
  assert(h < height);
  if(in_nodes[h] != NULL) {
    // Note that incoming node references are not counted, this is on purpose.
    // If incoming references are the only ones left, the node should
    // be deleted.
    in_nodes[h] = NULL;
  }
  // success
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
// PersistentSkipList Implementation                                         //
///////////////////////////////////////////////////////////////////////////////

template <class T>
PersistentSkipList<T>::PersistentSkipList()
  : height(0), present(0), head(), data_set()
{
  if(PSL_DEBUG_MODE) {
    clog << "PSL " << this << " created." << endl;
  }
}

template <class T>
PersistentSkipList<T>::~PersistentSkipList() {
  for(int i = 0; i < (int)head.size(); ++i) {
    TSA* tsa = head[i];
    if(PSL_DEBUG_MODE) {
      clog << "Deleting head at time " << i << endl;
    }
    for(int j = 0; j < tsa->getSize(); ++j) {
      ListNode<T>* ln = tsa->getElement(j);
      if(PSL_DEBUG_MODE) {
	clog << "Removing head pointer to node(" << ln->getData() << ")" << endl;
      }
    }
    delete tsa;
  }
  if(PSL_DEBUG_MODE) {
    clog << "PSL " << this << " deleted." << endl;
  }
}

template <class T>
int PersistentSkipList<T>::getPresent() const {
  assert(this != NULL);
  return present;
}

template <class T>
PersistentSkipList<T>& PersistentSkipList<T>::operator++() {
  incTime();
  return this;
}

template <class T>
void PersistentSkipList<T>::incTime() {
  assert(this != NULL);
  TSA* old_head = getHead(getPresent());
  old_head->lock();
  ++present;
  TSA* new_head = new TSA(getPresent(),old_head->getSize(),*old_head);
  addHead(new_head);
}

template <class T>
void PersistentSkipList<T>::drawPresent() {
  assert(this != NULL);
  draw(getPresent());
}

template <class T>
void PersistentSkipList<T>::draw(int t) {
  assert(this != NULL);
  assert(t >= 0);
  const int WIDTH = 16;
  cout << "Getting skip list at time " << t << "..." << endl;
  TSA* head = getHead(t);
  if(head == NULL) {
    cout << "NULL" << endl;
    return;
  }
  vector<int> heights;
  vector<T> data;
  ListNode<T>* ln = head->getElement(0);
  int max_height = -1;
  while(ln != NULL) {
    int h = ln->getHeight();
    heights.push_back(h);
    if(h > max_height)
      max_height = h;
    data.push_back(ln->getData());
    ln = ln->getNext(t,0);
  }
  --max_height;
  while(max_height >= 0) {
    cout << max_height << ": ";
    for(int i = 0; i < (int)heights.size(); ++i) {
      cout << setw(WIDTH);
      if(heights[i] > max_height) {
	cout << data[i];
      } else {
	cout << "   ";
      }
      cout << " -> ";
    }
    cout << "NULL" << endl;
    --max_height;
  }
}

template <class T>
int PersistentSkipList<T>::addHead(TimeStampedArray<ListNode<T>*>* tsa) {
  assert(this != NULL);
  assert(tsa != NULL);
  // save the new head
  head.push_back(tsa);
  // success
  return 0;
}

template <class T>
TimeStampedArray<ListNode<T>*>* PersistentSkipList<T>::getHead(int t) {
  assert(this != NULL);
  int index = -1;
  int begin = 0, end = head.size() -1;
  int timeFound = 0;
  // binary search
  while(begin <= end) {
    index = (begin+end)/2;
    timeFound = head[index]->getTime();
    if(timeFound == t) {
      // done, break out of loop
      break;
    } else if(timeFound > t) {
      // repeat binary search on left (earlier) half
      end = index -1;
    } else {
      // repeat binary search on right (later) half
      begin = index +1;
    }
  }
  // closest may be after given time
  while(index > 0 && head[index]->getTime() > t)
    --index;
  if(index < 0)
    return NULL;
  // finally, return the found element
  return head[index];
}

template <class T>
int PersistentSkipList<T>::setHead(TimeStampedArray<ListNode<T>*>* tsa) {
  assert(this != NULL);
  assert(tsa != NULL);
  // find where the head needs to be inserted
  typename vector<TSA*>::iterator iter = head.begin();
  // linear search
  while( iter < head.end() && (*iter)->getTime() < tsa->getTime() )
    ++iter;
  // set
  if( iter < head.end() && (*iter)->getTime() == tsa->getTime() ) {
    TSA* temp = *iter;
    *iter = tsa;
    delete temp;
  }
  // insert
  else {
    cout << "Inserting head" << endl;
    head.insert(iter,tsa);
  }
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// INSERT METHOD                                                           //
/////////////////////////////////////////////////////////////////////////////

template <class T>
int PersistentSkipList<T>::initialInsert(const T& data) {
  TSA* new_head = NULL;
  ListNode<T>* new_ln = ListNode<T>::create(data);
  int height = new_ln->getHeight();
  // initialize head
  new_head = new TSA(0, height);
  // make the new node the head at all heights
  for(int i = 0; i < height; ++i) {
    new_head->setElement(i,new_ln);
  }
  // since we created a new head, lock it then add it to the array of heads
  setHead(new_head);
  // prevent duplicates by registering this datum
  data_set.insert(data);
  return 0;
}

template <class T>
const PersistentSkipList<T>& PersistentSkipList<T>::operator+=(const T& data) {
  if(insert(data) != 0) // error
    throw "Unable to insert data!";
  return this;
}

template <class T>
int PersistentSkipList<T>::insert(const T& data) {
  assert(this != NULL);
  // check if data exists already
  if(data_set.count(data)>0)
    return 1;
  // if it's the first element in the skiplist
  if(data_set.empty())
    return initialInsert(data);
  // otherwise, create node
  TSA* curr_head = getHead(getPresent());
  ListNode<T>* new_ln = ListNode<T>::create(data);
  int height = new_ln->getHeight();
  if(PSL_DEBUG_MODE) {
    clog << "New node (" << data << ") height: " << height << endl;
  }
  // add node to list
  int start = height-1;
  /////////////////////////////////////////////////////////////////////////
  // TALLER THAN OLD HEAD                                                //
  /////////////////////////////////////////////////////////////////////////
  if(height > curr_head->getSize()) {
    if(PSL_DEBUG_MODE) {
      clog << "Node " << data << " is taller than old head." << endl;
    }
    TSA* new_head = new TSA(getPresent(),height,*curr_head);
    // make the new node the head at all heights exceeding the size of
    // the old head
    while(start >= curr_head->getSize()) {
      new_head->setElement(start,new_ln);
      --start;
    }
    setHead(new_head);
    curr_head = new_head;
  }
  TSA* new_node_next = new TSA(getPresent(),height);
  /////////////////////////////////////////////////////////////////////////
  // ADD TO HEAD IF NEEDED                                               //
  /////////////////////////////////////////////////////////////////////////
  ListNode<T>* old_ln = curr_head->getElement(start);
  // travel down the heads, adding the new node until we find a head
  // node which precedes the new node
  while(data < old_ln->getData()) {
    if(PSL_DEBUG_MODE) {
      clog << "Linking new to old at height " << start << endl;
    }
    old_ln->addIncomingNode(start,new_ln);
    new_node_next->setElement(start,old_ln);
    curr_head->setElement(start,new_ln);
    --start;
    if(start < 0)
      break;
    old_ln = curr_head->getElement(start);
  }
  /////////////////////////////////////////////////////////////////////////
  // ADD TO REST OF LIST IF NEEDED                                       //
  /////////////////////////////////////////////////////////////////////////
  if(start >= 0) {
    // guaranteed not NULL
    ListNode<T>* old_ln = curr_head->getElement(start);
    while(start >= 0) {
      // might be NULL
      ListNode<T>* next_ln = old_ln->getNext(getPresent(),start);
      // find the elements between which we should insert the new node
      while(next_ln != NULL && new_ln->getData() > next_ln->getData()) {
	old_ln = next_ln;
	next_ln = old_ln->getNext(getPresent(),start);
      }
      // add node to preceding node
      TSA* old_ln_next =
	old_ln->getNext(getPresent());  // <- this could be NULL
      int old_ln_height = old_ln->getHeight();
      if(old_ln_next == NULL) {
	old_ln_next =
	  new TSA(getPresent(),old_ln_height);
      }
      while(next_ln == NULL || new_ln->getData() < next_ln->getData()) {
	// point the new node to the old next node
	if(next_ln != NULL) {
	  next_ln->removeIncomingNode(start);
	  next_ln->addIncomingNode(start,new_ln);
	  new_node_next->setElement(start,next_ln);
	}
	// point the old node to the new node
	new_ln->removeIncomingNode(start);
	new_ln->addIncomingNode(start,old_ln);
	old_ln_next->setElement(start,new_ln);
	// move to the next height
	--start;
	if(start < 0)
	  break;
	next_ln = old_ln_next->getElement(start);
      }
      if(old_ln->getNext(getPresent()) == NULL)
	old_ln->addNext(old_ln_next);
      // move to next search height
      if(start < 0)
	break;
      // get node at new height
      old_ln = next_ln;
    }
  }
  new_ln->addNext(new_node_next);
  // prevent duplicates by registering this datum
  data_set.insert(data);
  // success
  return 0;
}

#endif
