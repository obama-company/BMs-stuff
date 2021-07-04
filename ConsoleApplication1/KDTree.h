#pragma once

#include <stdexcept>
#include <cmath>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include "Point.h"
#include "BoundedPQueue.h"

#define N 2

template <typename ElemType>
class KDTree
{
public:
    KDTree ();
    KDTree (std::vector<std::pair<Point, ElemType>>& points);
    KDTree (const KDTree& rhs);

    ~KDTree ();
    
    KDTree& operator=(const KDTree& rhs);

    std::size_t dimension () const;

    std::size_t size () const;
    bool empty () const;

    bool contains (const Point& pt) const;

    void insert (const Point& pt, const ElemType& value = ElemType ());

    ElemType& operator[](const Point& pt);

    ElemType kNNValue (const Point& key, std::size_t k) const;

private:
    struct Node
    {
        Point point;
        Node* left;
        Node* right;
        int level;
        ElemType value;
        Node (const Point& _pt, int _level, const ElemType& _value = ElemType ()) :
            point (_pt), left (NULL), right (NULL), level (_level), value (_value) {}
    };

    Node* root_;
    std::size_t size_;

    Node* buildTree (typename std::vector<std::pair<Point, ElemType>>::iterator start,
        typename std::vector<std::pair<Point, ElemType>>::iterator end, int currLevel);

    Node* findNode (Node* currNode, const Point& pt) const;

    void nearestNeighborRecurse (const Node* currNode, const Point& key, BoundedPQueue<ElemType>& pQueue) const;

    Node* deepcopyTree (Node* root);

    void freeResource (Node* currNode);
};


template <typename ElemType>
KDTree<ElemType>::KDTree () :
    root_ (NULL), size_ (0)
{
}

template <typename ElemType>
typename KDTree<ElemType>::Node* KDTree<ElemType>::deepcopyTree (typename KDTree<ElemType>::Node* root)
{
    if (root == NULL) return NULL;
    Node* newRoot = new Node (*root);
    newRoot->left = deepcopyTree (root->left);
    newRoot->right = deepcopyTree (root->right);
    return newRoot;
}

template <typename ElemType>
typename KDTree<ElemType>::Node* KDTree<ElemType>::buildTree (typename std::vector<std::pair<Point, ElemType>>::iterator start,
    typename std::vector<std::pair<Point, ElemType>>::iterator end, int currLevel)
{
    if (start >= end) return NULL; // empty tree

    int axis = currLevel % N; // the axis to split on
    auto cmp = [axis](const std::pair<Point, ElemType>& p1, const std::pair<Point, ElemType>& p2) {
        return p1.first[axis] < p2.first[axis];
    };
    std::size_t len = end - start;
    auto mid = start + len / 2;
    std::nth_element (start, mid, end, cmp); // linear time partition

    // move left (if needed) so that all the equal points are to the right
    // The tree will still be balanced as long as there aren't many points that are equal along each axis
    while (mid > start && (mid - 1)->first[axis] == mid->first[axis]) {
        --mid;
    }

    Node* newNode = new Node (mid->first, currLevel, mid->second);
    newNode->left = buildTree (start, mid, currLevel + 1);
    newNode->right = buildTree (mid + 1, end, currLevel + 1);
    return newNode;
}

template <typename ElemType>
KDTree<ElemType>::KDTree (std::vector<std::pair<Point, ElemType>>& points)
{
    root_ = buildTree (points.begin (), points.end (), 0);
    size_ = points.size ();
}

template <typename ElemType>
KDTree<ElemType>::KDTree (const KDTree& rhs)
{
    root_ = deepcopyTree (rhs.root_);
    size_ = rhs.size_;
}

template <typename ElemType>
KDTree<ElemType>& KDTree<ElemType>::operator=(const KDTree& rhs)
{
    if (this != &rhs) { // make sure we don't self-assign
        freeResource (root_);
        root_ = deepcopyTree (rhs.root_);
        size_ = rhs.size_;
    }
    return *this;
}

template <typename ElemType>
void KDTree<ElemType>::freeResource (typename KDTree<ElemType>::Node* currNode)
{
    if (currNode == NULL) return;
    freeResource (currNode->left);
    freeResource (currNode->right);
    delete currNode;
}

template <typename ElemType>
KDTree<ElemType>::~KDTree ()
{
    freeResource (root_);
}

template <typename ElemType>
std::size_t KDTree<ElemType>::dimension () const
{
    return N;
}

template <typename ElemType>
std::size_t KDTree<ElemType>::size () const
{
    return size_;
}

template <typename ElemType>
bool KDTree<ElemType>::empty () const
{
    return size_ == 0;
}

template <typename ElemType>
typename KDTree<ElemType>::Node* KDTree<ElemType>::findNode (typename KDTree<ElemType>::Node* currNode, const Point& pt) const
{
    if (currNode == NULL || currNode->point == pt) return currNode;

    const Point& currPoint = currNode->point;
    int currLevel = currNode->level;
    if (pt[currLevel % N] < currPoint[currLevel % N]) { // recurse to the left side
        return currNode->left == NULL ? currNode : findNode (currNode->left, pt);
    } else { // recurse to the right side
        return currNode->right == NULL ? currNode : findNode (currNode->right, pt);
    }
}

template <typename ElemType>
bool KDTree<ElemType>::contains (const Point& pt) const
{
    auto node = findNode (root_, pt);
    return node != NULL && node->point == pt;
}

template <typename ElemType>
void KDTree<ElemType>::insert (const Point& pt, const ElemType& value)
{
    auto targetNode = findNode (root_, pt);
    if (targetNode == NULL) { // this means the tree is empty
        root_ = new Node (pt, 0, value);
        size_ = 1;
    } else {
        if (targetNode->point == pt) { // pt is already in the tree, simply update its value
            targetNode->value = value;
        } else { // construct a new node and insert it to the right place (child of targetNode)
            int currLevel = targetNode->level;
            Node* newNode = new Node (pt, currLevel + 1, value);
            if (pt[currLevel % N] < targetNode->point[currLevel % N]) {
                targetNode->left = newNode;
            } else {
                targetNode->right = newNode;
            }
            ++size_;
        }
    }
}

template <typename ElemType>
ElemType& KDTree<ElemType>::operator[](const Point& pt)
{
    auto node = findNode (root_, pt);
    if (node != NULL && node->point == pt) { // pt is already in the tree
        return node->value;
    } else { // insert pt with default ElemType value, and return reference to the new ElemType
        insert (pt);
        if (node == NULL) return root_->value; // the new node is the root
        else return (node->left != NULL && node->left->point == pt) ? node->left->value : node->right->value;
    }
}

template <typename ElemType>
void KDTree<ElemType>::nearestNeighborRecurse (const typename KDTree<ElemType>::Node* currNode, const Point& key, BoundedPQueue<ElemType>& pQueue) const
{
    if (currNode == NULL) return;
    const Point& currPoint = currNode->point;

    // Add the current point to the BPQ if it is closer to 'key' that some point in the BPQ
    pQueue.enqueue (currNode->value, distance (currPoint, key));

    // Recursively search the half of the tree that contains Point 'key'
    int currLevel = currNode->level;
    bool isLeftTree;
    if (key[currLevel % N] < currPoint[currLevel % N]) {
        nearestNeighborRecurse (currNode->left, key, pQueue);
        isLeftTree = true;
    } else {
        nearestNeighborRecurse (currNode->right, key, pQueue);
        isLeftTree = false;
    }

    if (pQueue.size () < pQueue.maxSize () || fabs (key[currLevel % N] - currPoint[currLevel % N]) < pQueue.worst ()) {
        // Recursively search the other half of the tree if necessary
        if (isLeftTree) nearestNeighborRecurse (currNode->right, key, pQueue);
        else nearestNeighborRecurse (currNode->left, key, pQueue);
    }
}

template <typename ElemType>
ElemType KDTree<ElemType>::kNNValue (const Point& key, std::size_t k) const
{
    BoundedPQueue<ElemType> pQueue (k); // BPQ with maximum size k
    if (empty ()) return ElemType (); // default return value if KD-tree is empty

    // Recursively search the KD-tree with pruning
    nearestNeighborRecurse (root_, key, pQueue);

    // Count occurrences of all ElemType in the kNN set
    std::map<ElemType, int> counter;
    while (!pQueue.empty ()) {
        ++counter[pQueue.dequeueMin ()];
    }

    // Return the most frequent element in the kNN set
    ElemType result;
    int cnt = -1;
    for (const auto& p : counter) {
        if (p.second > cnt) {
            result = p.first;
            cnt = p.second;
        }
    }
    return result;
}