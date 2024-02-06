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

// 트리에서 주어진 노드를 삭제하는 함수
// TODO: 삭제 구현
int rbtree_erase(rbtree *t, node_t *p)
{
    // 1. 삭제할 노드 찾기 (rbtree_find 사용)
    // 2. 노드 삭제 후 Red-Black 트리 속성 유지를 위한 조정 작업 필요
    return 0;
}

// 트리의 모든 키를 배열로 변환하는 함수
// TODO: 트리가 배열이 되도록 구현
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
    // 1. 트리를 중위 순회하면서 각 노드의 키를 배열에 저장
    // 2. 순회한 순서대로 배열에 키 저장
    return 0;
}