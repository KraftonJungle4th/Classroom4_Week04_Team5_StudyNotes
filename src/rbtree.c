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

// 트리에 새로운 키를 가진 노드를 삽입하는 함수
// TODO: 삽입 구현
node_t *rbtree_insert(rbtree *t, const key_t key)
{
    // 1. 일반 이진 탐색 트리처럼 노드 삽입
    // 2. 삽입된 노드의 색상을 빨간색으로 설정
    // 3. Red-Black 트리의 속성을 유지하기 위해 삽입 후 조정 작업 필요
    return t->root;
}

// 트리에서 주어진 키를 가진 노드를 찾는 함수
// TODO: 찾기 구현
node_t *rbtree_find(const rbtree *t, const key_t key)
{
    // 1. 루트 노드부터 시작하여 키 비교를 통해 왼쪽 또는 오른쪽 자식으로 이동
    // 2. 일치하는 키를 찾으면 해당 노드 반환, 찾지 못하면 NULL 반환
    return t->root;
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