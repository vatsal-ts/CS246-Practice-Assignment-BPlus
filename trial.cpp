// C++ program for implementing B+ Tree
/* #region typedefs faster code hehe  */
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
// using namespace std;
#define gc getchar_unlocked
#define fo(i, n) for (auto i = 0; i < n; i++)
#define Fo(i, k, n) for (auto i = k; k < n ? i < n : i > n; k < n ? i += 1 : i -= 1)
#define ll long long
#define si(x) scanf("%d", &x)
#define sl(x) scanf("%lld", &x)
#define ss(s) scanf("%s", s)
#define pi(x) printf("%d\n", x)
#define pl(x) printf("%lld\n", x)
#define ps(s) printf("%s\n", s)
#define deb(x) cout << #x << "=" << x << endl
#define deb2(x, y) cout << #x << "=" << x << "," << #y << "=" << y << endl
#define pb push_back
#define mp make_pair
#define F first
#define S second
#define all(x) x.begin(), x.end()
#define clr(x) memset(x, 0, sizeof(x))
#define sortall(x) sort(all(x))
#define tr(it, a) for (auto it = a.begin(); it != a.end(); it++)
#define PI 3.1415926535897932384626
typedef pair<int, int> pii;
typedef pair<ll, ll> pl;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<ll> vll;
typedef vector<pii> vpii;
typedef vector<pl> vpl;
typedef vector<pll> vpll;
typedef vector<vi> vvi;
typedef vector<vl> vvl;
typedef vector<vll> vvll;
#define lb lower_bound
#define ub upper_bound
#define PRESENT -1
#define FULL 0
#define SUCCESS 1
#define fast                          \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);                    \
    cout.tie(NULL)
/* #endregion */
// int MAX = 3;
int leaf_size = 3;
int indexnode_size = 3;
int data_nodes = 0, index_nodes = 0;
// max=2d+1
// BP node

class Node
{
public:
    bool IS_LEAF;
    int *key, size;      // key is an array(hence the *) of values
    Node **ptr;          // ptr is the array(1st *) of pointers(2nd *) to other nodes
    friend class BPTree; // BPTree can now access private members of Node

    Node(int);
};

// BP tree
class BPTree
{
    Node *root; // only has the pointer to root Node
    void insertInternal(int, Node *, Node *);
    Node *findParent(Node *, Node *);

public:
    BPTree();
    void search(int);
    void insert(int);
    void display(Node *);
    Node *getRoot();
};

// Constructor of Node
Node::Node(int n)
{
    key = new int[n];
    ptr = new Node *[n + 1]; // array of pointers to other nodes
}

// Initialise the BPTree Node
BPTree::BPTree()
{
    root = NULL;
}

// Function to find any element
// in B+ Tree
void BPTree::search(int x)
{

    // If tree is empty
    if (root == NULL)
    {
        cout << "Tree is empty\n";
    }

    // Traverse to find the value
    else
    {

        Node *cursor = root;

        // Till we reach leaf node
        while (cursor->IS_LEAF == false)
        {

            for (int i = 0;
                 i < cursor->size; i++)
            {

                // If the element to be
                // found is not present
                if (x < cursor->key[i])
                {
                    cursor = cursor->ptr[i];
                    break;
                }

                // If reaches end of the
                // cursor node
                if (i == cursor->size - 1)
                {
                    cursor = cursor->ptr[i + 1];
                    break;
                }
            }
        }

        // Traverse the cursor and find
        // the node with value x
        for (int i = 0;
             i < cursor->size; i++)
        {

            // If found then return
            if (cursor->key[i] == x)
            {
                cout << "Found\n";
                return;
            }
        }

        // Else element is not present
        cout << "Not found\n";
    }
}

// Function to implement the Insert
// Operation in B+ Tree
void BPTree::insert(int x)
{

    // If root is null then return
    // newly created node
    if (root == NULL)
    {
        root = new Node(leaf_size); // key and ptr are both initialised
        root->key[0] = x;
        root->IS_LEAF = true; // first creation root is leaf
        root->size = 1;       // initially sized 1
        data_nodes++;
    }

    // Traverse the B+ Tree
    else
    {
        Node *cursor = root;
        Node *parent;
        // Till cursor reaches the
        // leaf node
        while (cursor->IS_LEAF == false)
        {
            parent = cursor;
            // will keep track of the exact parent for the leaf
            // node where insertion is being done
            for (int i = 0; i < cursor->size; i++)
            {

                // If found the position
                // where we have to insert
                // node
                if (x < cursor->key[i])
                {
                    cursor = cursor->ptr[i];
                    break;
                }

                // If reaches the end
                if (i == cursor->size - 1)
                {
                    cursor = cursor->ptr[i + 1];
                    break;
                }
            }
        }

        if (cursor->size < leaf_size) // we are in leaf node now & it has space, not full
        {
            // cout<<"lol"<<x;
            int i = 0;
            while (x > cursor->key[i] && i < cursor->size)
            {
                i++; // i is now the index that the new insertion must take
            }

            for (int j = cursor->size; j > i; j--)
            {
                cursor->key[j] = cursor->key[j - 1]; // ith position shifted to i+1
            }

            cursor->key[i] = x; // inserted at that position
            cursor->size++;     // inserted succesfully

            cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
            // linked list shift next pointer to last position
            cursor->ptr[cursor->size - 1] = NULL;
        }

        else
        {
            // full leaf, split!
            // Create a newLeaf node
            Node *newLeaf = new Node(leaf_size);
            int virtualNode[leaf_size + 1]; // if the leaf node "could" hold that much

            // Update cursor to virtual
            // node created
            for (int i = 0; i < leaf_size; i++)
            {
                virtualNode[i] = cursor->key[i];
            }
            int i = 0, j;

            // Traverse to find where the new
            // node is to be inserted
            while (x > virtualNode[i] && i < leaf_size)
            {
                i++; // index which the new insertion wouldve taken
            }

            // Update the current virtual
            // Node to its previous
            for (int j = leaf_size + 1;
                 j > i; j--)
            {
                virtualNode[j] = virtualNode[j - 1]; // make space for new insertion
            }

            virtualNode[i] = x; // new insertion of leaf
            newLeaf->IS_LEAF = true;
            data_nodes++;

            cursor->size = (leaf_size + 1) / 2; // ceiling
            newLeaf->size = leaf_size + 1 - (leaf_size + 1) / 2;

            cursor->ptr[cursor->size] = newLeaf; // next of cursor pointer is newLeaf on the right

            newLeaf->ptr[newLeaf->size] = cursor->ptr[leaf_size]; // next pointer shift linked list

            cursor->ptr[leaf_size] = NULL; // earlier next is removed linked list

            // cursor->next=newLeaf
            //  Update the current virtual
            //  Node's key to its previous
            for (i = 0; i < cursor->size; i++)
            {
                cursor->key[i] = virtualNode[i];
            } // cursor re-set

            // Update the newLeaf key to
            // virtual Node
            for (i = 0, j = cursor->size;
                 i < newLeaf->size;
                 i++, j++)
            {
                newLeaf->key[i] = virtualNode[j];
            } // newleaf re-set

            // If cursor is the root node
            if (cursor == root) // no parent then
            {                   // if root was being split

                // Create a new Node
                Node *newRoot = new Node(indexnode_size);

                // Update rest field of
                // B+ Tree Node
                newRoot->key[0] = newLeaf->key[0]; // following jenny scheme
                // https://www.youtube.com/watch?v=DqcZLulVJ0M
                newRoot->ptr[0] = cursor;
                newRoot->ptr[1] = newLeaf;
                newRoot->IS_LEAF = false;
                index_nodes++;
                newRoot->size = 1;
                root = newRoot; // update root
            }
            else
            {

                // Recursive Call for
                // insert in internal
                // cout<<newLeaf->key[0]<<"getsent\n";
                // cout<<"newLeaf has:\n";
                // for(int i=0;i<newLeaf->size;i++){
                //     cout<<newLeaf->key[i]<<" ";
                // }
                // cout<<"\n";
                insertInternal(newLeaf->key[0],
                               parent,
                               newLeaf); // b tree insertion follow
            }
        }
    }
}

// Function to implement the Insert
// Internal Operation in B+ Tree
// B-Tree insertion here on out
void BPTree::insertInternal(int x, Node *cursor, Node *child)
{

    // If we don't have overflow
    if (cursor->size < indexnode_size)
    {
        // cout<<"um";
        int i = 0;

        // Traverse the child node
        // for current cursor node
        while (x > cursor->key[i] && i < cursor->size)
        {
            i++;
        }
        // i is the position the value shud be inserted
        //  Traverse the cursor node
        //  and update the current key
        //  to its previous node key
        for (int j = cursor->size;
             j > i; j--)
        {

            cursor->key[j] = cursor->key[j - 1]; // shift all values ahead
        }

        // Traverse the cursor node
        // and update the current ptr
        // to its previous node ptr
        // shift pointers as well
        for (int j = cursor->size + 1; // j will be all new indices of shifted pointers
             j > i + 1; j--)
        {
            cursor->ptr[j] = cursor->ptr[j - 1];
        }

        cursor->key[i] = x;
        cursor->size++;
        cursor->ptr[i + 1] = child; // put newly connected node appropriately
    }

    // For overflow, break the node
    else
    {
        // full internal node in B-Tree
        //  For new Interval
        Node *newInternal = new Node(indexnode_size);
        int virtualKey[indexnode_size + 1];
        Node *virtualPtr[indexnode_size + 2];

        // Insert the current list key
        // of cursor node to virtualKey
        for (int i = 0; i < indexnode_size; i++)
        {
            virtualKey[i] = cursor->key[i];
        }

        // Insert the current list ptr
        // of cursor node to virtualPtr
        for (int i = 0; i < indexnode_size + 1; i++)
        {
            virtualPtr[i] = cursor->ptr[i];
        }

        int i = 0, j;

        // Traverse to find where the new
        // node is to be inserted
        while (x > virtualKey[i] && i < indexnode_size)
        {
            i++;
        }

        // Traverse the virtualKey node
        // and update the current key
        // to its previous node key
        for (int j = indexnode_size + 1; j > i; j--)
        {

            virtualKey[j] = virtualKey[j - 1];
        }

        virtualKey[i] = x; // new insertion on virtual
        /*
        for(int i=0;i<indexnode_size+1;i++){
            cout<<virtualKey[i]<<"v";
        }
        cout<<"\n";
        */
        // Traverse the virtualKey node
        // and update the current ptr
        // to its previous node ptr
        // shifted pointers too
        for (int j = indexnode_size + 2;
             j > i + 1; j--)
        {
            virtualPtr[j] = virtualPtr[j - 1];
        }

        virtualPtr[i + 1] = child; // re-set child pointer
        newInternal->IS_LEAF = false;
        index_nodes++;
        cursor->size = (indexnode_size) / 2; // changing size of this node where insertion is being done

        newInternal->size = indexnode_size - (indexnode_size) / 2;
        
        //Pranshu's corrections
        for (int copy = 0; copy < cursor->size; copy++)
            {
                cursor->key[copy] = virtualKey[copy];
            } // cursor re-set
        for (i = 0, j = 0; // no pointer is skipped though
             i < cursor->size + 1;
             i++, j++)
        {
            cursor->ptr[i] = virtualPtr[j];
        }

        // Insert new node as an
        // internal node
        for (i = 0, j = cursor->size + 1; // important, see closely j is skipping virtalKey[cursor_size]
             i < newInternal->size;
             i++, j++)
        {

            newInternal->key[i] = virtualKey[j];
        }

        for (i = 0, j = cursor->size + 1; // no pointer is skipped though
             i < newInternal->size + 1;
             i++, j++)
        {

            newInternal->ptr[i] = virtualPtr[j];
        }

        // If cursor is the root node
        if (cursor == root)
        {

            // Create a new root node
            Node *newRoot = new Node(indexnode_size);

            // Update key value
            newRoot->key[0] = virtualKey[cursor->size];

            // Update rest field of
            // B+ Tree Node
            newRoot->ptr[0] = cursor;
            newRoot->ptr[1] = newInternal;
            newRoot->IS_LEAF = false;
            index_nodes++;
            newRoot->size = 1;
            root = newRoot;
        }

        else
        {

            // Recursive Call to insert
            // the data
            // cout<<virtualKey[cursor->size]<<"gets sent\n";
            insertInternal(virtualKey[cursor->size], // out of indedx but still holds a value which is to be pushed up
                           findParent(root,
                                      cursor),
                           newInternal);
        }
    }
}

// Function to find the parent node
Node *BPTree::findParent(Node *cursor,
                         Node *child)
{
    Node *parent;

    // If cursor reaches the end of Tree
    if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF)
    {
        return NULL;
    }

    // Traverse the current node with
    // all its child
    for (int i = 0;
         i < cursor->size + 1; i++)
    {

        // Update the parent for the
        // child Node
        if (cursor->ptr[i] == child)
        {
            parent = cursor;
            return parent;
        }

        // Else recursively traverse to
        // find child node
        else
        {
            parent = findParent(cursor->ptr[i],
                                child); // forced exploration of full subtree

            // If parent is found, then
            // return that parent node
            if (parent != NULL)
                return parent;
        }
    }

    // Return parent node
    return parent;
}

// Function to get the root Node
Node *BPTree::getRoot()
{
    return root;
}

int main()
{
    fast;
    int x, y;
    cin >> x >> y;
    leaf_size = 2 * x;
    indexnode_size = 2 * y + 1;
    x = -1;
    y = -1;
    BPTree node;
    while (x != 3)
    {
        cin >> x;
        if (x == 1)
        {
            cin >> y;
            node.insert(y);
        }
        else if (x == 2)
        {
            cout << index_nodes << " " << data_nodes << " ";
            Node *temp = node.getRoot();
            for (int i = 0; i < temp->size; i++)
            {
                cout << temp->key[i] << " ";
            }
            cout << "\n";
        }
    }

    // node.search(16);

    return 0;
}
