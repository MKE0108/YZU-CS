// xtree internal header
#define Red 0
#define Black 1
#ifndef XTREE
#define XTREE

template< typename Ty >
struct TreeNode
{
    using NodePtr = TreeNode*;
    using value_type = Ty;

    NodePtr    left;   // left subtree, or smallest element if head
    NodePtr    parent; // parent, or root of tree if head
    NodePtr    right;  // right subtree, or largest element if head
    char       color;  // 0: Red, 1: Black; Black if head
    char       isNil;  // 1 only if head (also nil) node
    value_type myval;  // the stored value, unused if head
};

// CLASS TEMPLATE TreeVal
template< typename Ty >
class TreeVal
{
public:
    using NodePtr = TreeNode< Ty >*;

    using value_type = Ty;
    using size_type = size_t;

    TreeVal()
        : myHead(new TreeNode< value_type >),
        mySize(0)
    {
        myHead->left = myHead;
        myHead->parent = myHead;
        myHead->right = myHead;
        myHead->color = 1;
        myHead->isNil = 1;
    }

    ~TreeVal()
    {
        clear(myHead->parent);
        delete myHead;
    }
    //Link to 2 Node ( NewPar And Children ) ----- O
    //@param NewPar ---- the node that you want it to become parent
    //@param Child ----- the node that you want it to become child
    //@param flag ----- 'L' -> Child is NewPar Left Kid ,'R' -> Child is NewPar Right Kid 
    void LinkStart(TreeNode< value_type >* NewPar, TreeNode< value_type >* Child, char flag)
    {
        if (flag == 'L') NewPar->left = Child;
        if (flag == 'R') NewPar->right = Child;
        if (flag == 'P') NewPar->parent = Child;
        if (Child->isNil == 0) { // not head
            Child->parent = NewPar;
        }

    }

    //Return sibling of node
    //@param node ---- the node that you want to search its sibling
    //@param par ----- if "node" is not external node, you don't need to assign the node of "node" parent. Otherwise, you need to assign the node of "node" parent.
    TreeNode< value_type >* Sibling(TreeNode< value_type >* node, TreeNode< value_type >* par = nullptr)

    {
        if (node->isNil == 0) {
            par = node->parent;
        }
        if (par->left == node) {
            return par->right;
        }
        else {
            return par->left;
        }
        return myHead;//orphin
    }
    //Swap color of two node
    inline void SwapColor(TreeNode< value_type >* node1, TreeNode< value_type >* node2) {
        int tmp = node1->color;
        node1->color = node2->color;
        node2->color = tmp;
    }
    // Removes all elements from the set object (which are destroyed)
    void clear(TreeNode< value_type >* node)
    {
        if (!node->isNil) // node is not an external node
        {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    // rebalance for insertion; u points to the inserted node
    void reBalance(TreeNode< value_type >* u)
    {  // u->parent cannot be the root
        TreeNode< value_type >* pu = u->parent;
        TreeNode< value_type >* gu = pu->parent;
        TreeNode< value_type >* uncle = Sibling(pu); // uncle of u
        //Case 1 : XYr
        if (uncle->color == Red)
        {
            //gu is root          
            // pu->black
            // ubro->black
            if (gu == myHead->parent)
            {
                pu->color = Black;
                uncle->color = Black;
            }
            //gu is not root              
            // gu->red
            // pu->black
            // ubro->black
            // if color(gu->par) is Red
            //    call reblance(gu)
            else
            {

                gu->color = Red;
                pu->color = Black;
                uncle->color = Black;
                if (gu->parent->color == Red) reBalance(gu);
            }
        }
        //Case 2 : XYb
        else
        {
            //LXb
            if (pu->parent->left == pu)
            {
                //LLb
                // pu color -> black
                // gu color -> red
                // call LLR
                if (u->parent->left == u)
                {
                    pu->color = Black;
                    gu->color = Red;
                    LLRotation(pu);
                }
                //LRb
                // u color -> black
                // gu pu color -> red
                // call LRR
                else
                {
                    u->color = Black;
                    pu->color = Red;
                    gu->color = Red;
                    LRRotation(u);
                }
            }
            //RXb
            else
            {   //RRb
                // pu color -> black
                // gu color -> red
                // call RRR 
                if (u->parent->right == u)

                {
                    pu->color = Black;
                    gu->color = Red;
                    RRRotation(pu);
                }
                //RLb
                // u color -> black
                // gu pu color -> red
                // call RLR 
                else
                {
                    u->color = Black;
                    pu->color = Red;
                    gu->color = Red;
                    RLRotation(u);
                }
            }
        }


    }

    // rotate right at gu, where pu = gu->left and u = pu->left
    void LLRotation(TreeNode< value_type >* pu)
    {
        TreeNode< value_type >* gu = pu->parent;
        TreeNode< value_type >* guPar = gu->parent;
        LinkStart(gu, pu->right, 'L');
        LinkStart(pu, gu, 'R');

        if (guPar->isNil == 1) //gu is root
        {
            LinkStart(guPar, pu, 'P');
        }
        else //gu is not root
        {
            if (guPar->left == gu) LinkStart(guPar, pu, 'L');
            else if (guPar->right == gu) LinkStart(guPar, pu, 'R');
        }
    }

    // LR rotation; pu = gu->left and u = pu->right
    void LRRotation(TreeNode< value_type >* u)
    {
        TreeNode< value_type >* pu = u->parent;
        TreeNode< value_type >* gu = pu->parent;
        RRRotation(u);
        LLRotation(u);
    }

    // RL rotation; pu = gu->right and u = pu->left
    void RLRotation(TreeNode< value_type >* u)
    {
        TreeNode< value_type >* pu = u->parent;
        TreeNode< value_type >* gu = pu->parent;
        LLRotation(u);
        RRRotation(u);
    }

    // rotate left at gu, where pu = gu->right and u = pu->right
    void RRRotation(TreeNode< value_type >* pu)
    {
        TreeNode< value_type >* gu = pu->parent;
        TreeNode< value_type >* guPar = gu->parent;
        LinkStart(gu, pu->left, 'R');
        LinkStart(pu, gu, 'L');

        if (guPar->isNil == 1) //gu is root
        {
            LinkStart(guPar, pu, 'P');
        }
        else //gu is not root
        {
            if (guPar->left == gu) LinkStart(guPar, pu, 'L');
            else if (guPar->right == gu) LinkStart(guPar, pu, 'R');
        }
    }

    // erase p provided that the degree of p is at most one
    // p points to the node to be deleted
    // p == M in "Ch 10 Sec 3.pptx"
    void eraseDegreeOne(TreeNode< value_type >* p)
    {
        TreeNode< value_type >* child;
        if (p->left != myHead)
            child = p->left;
        else if (p->right != myHead)
            child = p->right;
        else
            child = myHead;

        if (p == myHead->parent) // p points to the root; Case 3 in "Ch 10 Sec 3.pptx"
        {
            // set child to be the new root
            child->color = Black;
            LinkStart(myHead, child, 'P');
        }
        else
        {
            TreeNode< value_type >* pp = p->parent;//pp is P

            if (pp->left == p) {
                LinkStart(pp, child, 'L');
            }
            else {
                LinkStart(pp, child, 'R');
            }

            if (p->color == 1)
                if (child->color == 0) // Case 2 in "Ch 10 Sec 3.pptx"
                    child->color = 1;
                else                   // Case 4 in "Ch 10 Sec 3.pptx"
                    fixUp(child, p->parent);
        }

        delete p;
        mySize--;
    }

    // rebalance for deletion; Case 4 in "Ch 10 Sec 3.pptx"
    void fixUp(TreeNode< value_type >* N, TreeNode< value_type >* P)
    {
        TreeNode< value_type >* S = Sibling(N, P);
        //Case 4 and 5 
        if (S->color == Black && S->right->color == Black && S->left->color == Black)
        {
            //Case 4
            if (P->color == Red)
            {
                S->color = Red;
                P->color = Black;
            }
            //Case 5
            else
            {
                S->color = Red;
                if (P->parent->isNil == 0) {
                    fixUp(P, P->parent);
                }
            }
        }
        else
        {
            if (P->left == N)
            {
                //Case 2.1
                if (S->color == Black && S->right->color == Red)
                {
                    SwapColor(S, P);
                    S->right->color = Black;
                    RRRotation(S);
                    return;
                }
                //Case 3.1
                if (S->color == Black && S->right->color == Black
                    && S->left->color == Red)
                {
                    SwapColor(S, S->left);
                    LLRotation(S->left);
                    fixUp(N, P);
                    return;
                }
                //Case 1.1
                if (S->color == Red)
                {
                    SwapColor(S, P);
                    RRRotation(S);
                    fixUp(N, P);
                    return;
                }
            }
            else
            {
                //Case 2.2
                if (S->color == Black && S->left->color == Red)
                {
                    SwapColor(S, P);
                    S->left->color = Black;
                    LLRotation(S);
                    return;
                }
                //Case 3.2
                if (S->color == Black && S->left->color == Black
                    && S->right->color == Red)
                {
                    SwapColor(S, S->right);
                    RRRotation(S->right);
                    fixUp(N, P);
                    return;
                }
                //Case 1.2
                if (S->color == Red)
                {
                    SwapColor(S, P);
                    LLRotation(S);
                    fixUp(N, P);
                    return;
                }

            }
        }


    }

    NodePtr myHead;   // pointer to head node
    size_type mySize; // number of elements
};

// CLASS TEMPLATE Tree
template< typename Traits >
class Tree // ordered red-black tree for map/multimap/set/multiset
{
public:
    using value_type = typename Traits::value_type;

protected:
    using ScaryVal = TreeVal< value_type >;

public:
    using key_type = typename Traits::key_type;
    using key_compare = typename Traits::key_compare;

    using size_type = size_t;

    Tree(const key_compare& parg)
        : keyCompare(parg),
        scaryVal()
    {
    }

    ~Tree()
    {
    }



    // Extends the container by inserting a new element,
    // effectively increasing the container size by one.
    void insert(const value_type& val)
    {
        if (scaryVal.mySize == 0)
        {
            TreeNode< value_type >* root = new TreeNode< value_type >;
            root->myval = val;
            root->left = scaryVal.myHead;
            root->right = scaryVal.myHead;
            root->isNil = 0;
            root->parent = scaryVal.myHead;
            root->color = 1;
            scaryVal.myHead->left = root;
            scaryVal.myHead->parent = root;
            scaryVal.myHead->right = root;
            scaryVal.mySize = 1;
        }
        else
        {                           // scaryVal.myHead->parent points to the root
            TreeNode< value_type >* p = scaryVal.myHead->parent; // p points to the root
            TreeNode< value_type >* pp = nullptr;
            while (p != scaryVal.myHead)
            {
                pp = p;
                //              keyCompare.operator()( val, p->myval )
                if (keyCompare(val, p->myval)) // if( val < p->myval )
                    p = p->left;
                else if (keyCompare(p->myval, val)) // if( val > p->myval )
                    p = p->right;
                else
                    return;
            }
            TreeNode< value_type >* NewNode = new TreeNode< value_type >;
            NewNode->myval = val;
            NewNode->left = scaryVal.myHead;
            NewNode->right = scaryVal.myHead;
            NewNode->isNil = 0;
            NewNode->parent = pp;
            NewNode->color = Red;
            if (keyCompare(val, pp->myval)) {
                pp->left = NewNode;
            }
            else {
                pp->right = NewNode;
            }

            if (pp->color == Red) {
                scaryVal.reBalance(NewNode);
            }

            if (keyCompare(val, scaryVal.myHead->left->myval))
            {
                scaryVal.myHead->left = NewNode;
            }
            if (keyCompare(scaryVal.myHead->right->myval, val))
            {
                scaryVal.myHead->right = NewNode;
            }
            scaryVal.mySize++;
            
        }
    }

    // Removes from the set container a single element whose value is val
    // This effectively reduces the container size by one, which are destroyed.
    // Returns the number of elements erased.
    size_type erase(const key_type& val)
    {
        TreeNode< key_type >* p = scaryVal.myHead->parent; // p points to the root
        while (p != scaryVal.myHead && val != p->myval)
        {
            //           keyCompare.operator()( val, p->myval )
            if (keyCompare(val, p->myval)) // if( val < p->myval )
                p = p->left;
            else
                p = p->right;
        }

        if (p == scaryVal.myHead) // not found
            return 0;
        else // found
        {  // deletes the node pointed by p
              // V
            bool rightf = 0;
            bool leftf = 0;

            TreeNode< key_type >* M = p;
            TreeNode< key_type >* itr = p->right;
            while (itr != scaryVal.myHead) {
                M = itr;
                itr = itr->left;
            }
            if (M == scaryVal.myHead->left) scaryVal.myHead->left = (M->right->isNil == 0)?M->right: M->parent;
            if (M == scaryVal.myHead->right) scaryVal.myHead->right = (M->left->isNil == 0) ? M->left : M->parent;

            p->myval = M->myval;
            
            scaryVal.eraseDegreeOne(M);
            return 1;
        }
    }

private:
    key_compare keyCompare;
    ScaryVal scaryVal;
};

#endif // XTREE