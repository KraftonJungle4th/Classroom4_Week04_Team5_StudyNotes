#include "rbtree.h"
#include <stdlib.h>

// 새로운 Red-Black 트리를 생성하고 초기화하는 함수
// TODO: 필요한 경우 구조체 초기화
rbtree *new_rbtree(void)
{
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

    if (!p)
    {
        // 메모리 할당 실패 처리
        return NULL;
    }

    // 센티넬(가드) 노드를 생성하고 검은색으로 설정
    p->nil = (node_t *)calloc(1, sizeof(node_t));

    if (!p->nil)
    {
        // 메모리 할당 실패 처리
        free(p); // 이미 할당된 p 메모리 해제
        return NULL;
    }

    p->nil->color = RBTREE_BLACK;
    p->root = p->nil;

    return p;
}

// 트리와 모든 노드의 메모리를 해제하는 함수
// TODO: 트리 노드의 메모리를 회수
void delete_rbtree(rbtree *t)
{
    // 모든 노드를 순회하면서 메모리 해제 필요

    // 후위 순회 방식을 사용해 자식 노드부터 메모리 해제 후, 루트 노드 해제
    free(t);
}

// 왼쪽으로 회전하는 함수
//   x        x
//  /    -->   \    .
// y            y
void rbtree_left_rotate(rbtree *t, node_t *x)
{
    node_t *y = x->right;

    x->right = y->left; // y의 왼쪽 서브트리를 x의 오른쪽 서브 트리로 옮기기

    if (y->left != t->nil)
    {
        y->left->parent = x;
    }
    y->parent = x->parent; // x의 부모를 y에 연결

    if (x->parent == t->nil)
    {
        t->root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    y->left = x; // x를 y의 왼쪽으로 놓기
    x->parent = y;
}

// 오른쪽으로 회전하는 함수
// rbtree_left_rotate랑 대칭
//   x        x
//  /    <--   \    .
// y            y
void rbtree_right_rotate(rbtree *t, node_t *x)
{
    node_t *y = x->left;

    x->left = y->right;

    if (y->right != t->nil)
    {
        y->right->parent = x;
    }
    y->parent = x->parent;

    if (x->parent == t->nil)
    {
        t->root = y;
    }
    else if (x == x->parent->right)
    {
        x->parent->right = y;
    }
    else
    {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *newNode)
{
    // 삽입한 노드부터 루트 노드까지 거슬러 올라가며 다음과 같은 경우를 고려
    while (newNode->parent->color == RBTREE_RED)
    {
        // 경우 1: 새로운 노드의 부모 노드가 조부모 노드의 왼쪽 자식인 경우
        if (newNode->parent == newNode->parent->parent->left)
        {
            // 조부모 노드, 삼촌 노드(부모 노드의 형제) 정의
            node_t *grandParent = newNode->parent->parent;
            node_t *uncle = grandParent->right;

            // 삼촌 노드 (부모 노드의 형제)가 빨간색인 경우:
            if (uncle->color == RBTREE_RED)
            {
                // 부모 노드와 삼촌 노드의 색깔을 빨간색에서 검은색으로 변경
                newNode->parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;

                // 조부모 노드의 색깔을 검은색에서 빨간색으로 변경
                grandParent->color = RBTREE_RED;
                newNode = grandParent;
            }
            // 삼촌 노드가 검은색인 경우:
            else
            {
                // 새로운 노드가 부모 노드의 오른쪽 자식인 경우에만
                if (newNode == newNode->parent->right)
                {
                    newNode = newNode->parent;
                    rbtree_left_rotate(t, newNode); // 왼쪽 회전을 수행
                }
                // 부모와 조부모 노드의 색을 변경한 후, 오른쪽 회전을 수행
                newNode->parent->color = RBTREE_BLACK;
                grandParent->color = RBTREE_RED;
                rbtree_right_rotate(t, newNode);
            }
        }

        // 경우 2: 새로운 노드의 부모 노드가 조부모 노드의 오른쪽 자식인 경우 (위의 경우를 좌우 반전)
        else
        {
            node_t *grandParent = newNode->parent->parent;
            node_t *uncle = grandParent->left;

            // 삼촌 노드 (부모 노드의 형제)가 빨간색인 경우:
            if (uncle->color == RBTREE_RED)
            {
                // 부모 노드와 삼촌 노드의 색깔을 빨간색에서 검은색으로 변경
                newNode->parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                // 조부모 노드의 색깔을 검은색에서 빨간색으로 변경
                grandParent->color = RBTREE_RED;
                newNode = grandParent;
            }
            else // 삼촌 노드가 검은색인 경우:
            {
                // 새로운 노드가 부모 노드의 왼쪽 자식인 경우에만
                if (newNode == newNode->parent->left)
                {
                    newNode = newNode->parent;
                    rbtree_right_rotate(t, newNode); // 오른쪽 회전을 수행
                }
                // 부모와 조부모 노드의 색을 변경한 후, 왼쪽 회전을 수행
                newNode->parent->color = RBTREE_BLACK;
                grandParent->color = RBTREE_RED;
                rbtree_left_rotate(t, newNode);
            }
        }
    }

    // 루트 노드의 색깔 설정: 레드-블랙 트리의 루트 노드를 검은색으로 설정하여 균형을 유지
    t->root->color = RBTREE_BLACK;
}

// 트리에 새로운 키를 가진 노드를 삽입하는 함수
node_t *rbtree_insert(rbtree *t, const key_t key)
{
    // 일반 이진 탐색 트리처럼 노드 삽입
    node_t *currentNode = t->root; // 루트 노드
    node_t *parentNode = t->nil; // 추후 부모가 될 노드

    // 루트 노드부터 내려가며 새로 노드가 삽입될 위치 찾기
    while (currentNode != t->nil)
    {
        parentNode = currentNode;

        if (key < currentNode->key)
        {
            currentNode = currentNode->left;
        }
        else
        {
            currentNode = currentNode->right;
        }
    }

    // 받은 key 값을 가진 노드 추가
    node_t *newNode = (node_t *)calloc(1, sizeof(node_t));
    if (!newNode)
    {
        // 메모리 할당 실패 처리
        return NULL;
    }

    // 삽입될 노드의 값 설정
    newNode->parent = parentNode;
    newNode->key = key;

    // 노드 삽입
    if (parentNode == t->nil)
    {
        t->root = newNode;
    }
    else if (newNode->key < parentNode->key)
    {
        parentNode->left = newNode;
    }
    else
    {
        parentNode->right = newNode;
    }

    // 삽입된 노드의 색상을 빨간색으로 설정
    newNode->color = RBTREE_RED;
    newNode->left = t->nil;
    newNode->right = t->nil;

    // Red-Black 트리의 속성을 유지하기 위해 삽입 후 조정 작업 필요
    rbtree_insert_fixup(t, newNode);
    return t->root;
}

// 트리에서 주어진 키를 가진 노드를 찾는 함수
// TODO: 찾기 구현
node_t *rbtree_find(const rbtree *t, const key_t key)
{
    node_t* node = NULL;
    node = t->root;
    // 1. 루트 노드부터 시작하여 키 비교를 통해 왼쪽 또는 오른쪽 자식으로 이동
    while (node != t->nil && node->key != key) {
        if (key < node->key) {
            node = node->left;
        }
        else {
            node = node->right;
        }
    }
        // 2. 일치하는 키를 찾으면 해당 노드 반환, 찾지 못하면 NULL 반환
    if (node == t->nil) {
        return NULL;
    }

    return node;
}

// 트리에서 가장 작은 키를 가진 노드를 찾는 함수
// TODO: 찾기 구현
node_t *rbtree_min(const rbtree *t)
{
    // 1. 루트 노드부터 시작하여 가장 왼쪽 노드까지 이동
    // 2. 가장 왼쪽 노드 반환
    return t->root;
}

// 트리에서 가장 큰 키를 가진 노드를 찾는 함수
// TODO: 찾기 구현
node_t *rbtree_max(const rbtree *t)
{
    // 1. 루트 노드부터 시작하여 가장 오른쪽 노드까지 이동
    // 2. 가장 오른쪽 노드 반환
    return t->root;
}

// 특정 서브 노드에서 가장 작은 값을 찾는 함수(노드보다 큰 값중 가장 작은 값 successor)
node_t *rbtree_minimum(const rbtree *t, node_t *y) {
    node_t *r = y;

    if(r == t->nil) {
        return r;
    }

    while (y->left != t->nil) { // y의 왼쪽 자식이 nil이 되기 전까지 반복
        y = y->left; // y의 왼쪽 자식값을 y에 담는다
    }
    return r; // r 반환
}

// 노드를 삭제 후, 삭제된 노드의 자식 노드들을 다른 노드에 연결하는 함수
void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
    if (u->parent == t->nil) { // 삭제된 노드의 부모 노드가 nil이라면(트리의 루트 노트인지 확인)
        t->root = v; // 루트 노드를 v로 설정(삭제된 노드의 자식 노드 중 하나)
    } else if(u == u->parent->left) { // 루트노드가 아니라면 삭제 노드가 부모노드의 왼쪽 자식인지 확인
        u->parent->left = v; // 왼쪽 자식을 v로 설정
    } else { // 그것도 아니라면
        u->parent->right = v; // 오른쪽 자식을 v로 설정
    }

    v->parent = u->parent; // v의 부모를 u의 부모로 설정(v가 u의 위치를 대체)
}

// 픽스업
void rbtree_erase_fixup(rbtree *t, node_t *x) {
    node_t *w;
    while((x != t->root) && (x->color == RBTREE_BLACK)) {
        if(x == x->parent->left) {
            w = x->parent->right;

            // case 1:
            if(w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rbtree_left_rotate(t, x->parent);
                w = x->parent->right;
            }

            // case 2:
            if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
        } else {
            // case 3:
            if(w->right->color == RBTREE_BLACK) {
                w->left->color = RBTREE_BLACK;
                w->color = RBTREE_RED;
                rbtree_right_rotate(t, w);
                w = x->parent->right;
            }

            // case 4:
            w->color = x->parent->color;
            x->parent->color = RBTREE_BLACK;
            w->right->color = RBTREE_BLACK;
            rbtree_left_rotate(t, x->parent);
            x = t->root;
            }
        } else {
            w = x->parent->left;
                
            // case 1:
            if(w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rbtree_right_rotate(t, x->parent);
                w = x->parent->left;
            }

            // case 2:
            if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            } else {
                // case 3:
                if(w->left->color == RBTREE_BLACK) {
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    rbtree_left_rotate(t, w);
                    w = x->parent->left;
                }

                // case 4:
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                rbtree_right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = RBTREE_BLACK;
}

// 트리에서 주어진 노드를 삭제하는 함수
// TODO: 삭제 구현
int rbtree_erase(rbtree *t, node_t *p)
{
    node_t *y = p;
    color_t y_original_color = y->color;
    node_t *x;

    if(p->left == t->nil) {
        x = p->right;
        rbtree_transplant(t, p, p->right);
    } else if(p->right == t->nil) {
        x = p->left;
        rbtree_transplant(t, p, p->left);
    } else {
        y = rbtree_minimum(t, p->right);
        y_original_color = y->color;
        x = y->right;

        if(y->parent == p) {
            x->parent = y;
        } else {
          rbtree_transplant(t, y, y->right);
          y->right = p->right;
          y->right->parent = y;  
        }

        rbtree_transplant(t, p, y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
    }

    if (y_original_color == RBTREE_BLACK) {
        rbtree_erase_fixup(t, x);
    }

    free(p);

    return 0;
}

// 중위 순회를 수행하며 키 값을 배열에 저장하는 함수
void inorder_traversal(node_t *node, int *index, int *arr) {
    // 현재 노드가 NULL이거나 인덱스가 유효하지 않으면 순회를 중단
    // if (node == NULL || *index <= -1)
    // {
    //     return;
    // }

    if (*index <= -1)
    {
        return;
    }

    // 왼쪽 서브트리를 순회
    inorder_traversal(node->left, index, arr);

    // 현재 노드를 방문. 현재 노드의 키를 배열의 현재 인덱스 위치에 저장하고, 인덱스를 증가
    if (*index >= 0)
    {
        arr[*index] = node->key;
        (*index)++;
    }

    // 오른쪽 서브트리를 순회
    inorder_traversal(node->right, index, arr);
}

// 레드-블랙 트리의 모든 키를 배열로 변환하는 함수
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // 배열이 NULL이거나 배열의 크기가 0이면 오류 코드를 반환
    // if (arr == NULL || n == 0) {
    //     return -1;
    // }

    if (n == 0) {
        return -1;
    }

    // 인덱스를 0으로 시작. 이는 배열에 키를 삽입하는 데 사용
    int index = 0;

    // 트리의 루트부터 시작하여 중위 순회를 수행
    // 이 순회는 트리의 키를 정렬된 순서로 배열에 채움
    inorder_traversal(t->root, &index, arr);

    // 순회가 끝난 후, 인덱스는 배열에 저장된 키의 수와 같아야 함
    // 인덱스가 배열의 크기를 초과하면 오류 코드를 반환
    if (index > n) {
        free(arr);
        return -1; // 배열의 크기보다 더 많은 노드가 트리에 있다면 오류 코드 반환
    }
    
    // 함수 작동의 성공을 반환
    return 0;
}