#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 20
typedef struct QueueType {
    int front;
    int rear;
    int data[MAX_QUEUE_SIZE];
} QueueType;
//==================================================
// 큐 관련 함수들
void init_queue(QueueType* q) {
    q->rear = -1;
    q->front = -1;
}
int is_empty_queue(QueueType* q) { return q->front == q->rear; }
int is_full_queue(QueueType* q) { return q->rear == MAX_QUEUE_SIZE - 1; }
void enqueue(QueueType* q, int item) {
    if (is_full_queue(q)) return;

    q->rear++;
    q->data[q->rear] = item;
}
int dequeue(QueueType* q) {
    if (is_empty_queue(q)) return INT32_MIN;

    q->front++;
    return q->data[q->front];
}
// 큐 관련 함수들 끝
//==================================================

#define MAX_VERTICES 256
typedef struct ListNode {
    int vertex;
    struct ListNode* link;
} ListNode;
typedef struct GraphType {
    int n;                            // 정점의 개수
    char vertex_label[MAX_VERTICES];  // 정점의 이름
    ListNode* adj_list[MAX_VERTICES];
} GraphType;
//==================================================
// 그래프 관련 함수들
int is_empty(GraphType* g) { return (g->n == 0); }
int is_full(GraphType* g) { return (g->n >= MAX_VERTICES); }

// [TODO 1] 그래프 초기화 함수
void init_graph(GraphType* g) {
    // 아직 정점이 하나도 없으므로 0으로 설정합니다.
    g->n = 0;
    
    // 모든 정점의 친구 목록(인접 리스트)을 비워둡니다.
    // NULL은 '아무것도 연결되지 않음'을 뜻해요. (PDF 19p 참조) 
    for (int i = 0; i < MAX_VERTICES; i++) {
        g->adj_list[i] = NULL;
    }
}

// [TODO 2] 그래프 메모리 해제 함수
void free_graph(GraphType* g) {
    // 그래프에 있는 모든 정점을 하나씩 돌면서 확인합니다.
    for (int i = 0; i < g->n; i++) {
        // i번째 정점의 친구 목록(연결 리스트)을 가져옵니다.
        ListNode* curr = g->adj_list[i];
        
        // 목록이 끝날 때까지 계속 반복합니다.
        while (curr != NULL) {
            ListNode* temp = curr; // 현재 노드를 임시로 저장해두고,
            curr = curr->link;     // 다음 노드로 이동한 뒤,
            free(temp);            // 아까 저장해둔 노드를 삭제(메모리 해제)합니다.
        }
    }
}

// [TODO 3] 정점 삽입 함수
void insert_vertex(GraphType* g, char v_label) {
    // 그래프가 꽉 찼으면 더 이상 넣을 수 없어요.
    if (is_full(g)) return;
    
    // 현재 정점 번호(g->n) 위치에 이름(A, B, C...)을 저장합니다.
    g->vertex_label[g->n] = v_label;
    
    // 정점의 개수를 1개 늘려줍니다.
    g->n++;
}

// [TODO 4] 간선 삽입 함수 (누구와 누가 연결되었는지 추가)
void insert_edge(GraphType* g, int from, int to) {
    // 새로운 친구(노드)를 하나 만듭니다. (동적 메모리 할당)
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    
    // 이 노드는 'to'라는 정점 번호를 담고 있어요.
    node->vertex = to;
    
    // [중요] 연결 리스트의 '맨 앞'에 새 친구를 끼워 넣습니다.
    // 1. 새 노드가 원래 있던 첫 번째 노드를 가리키게 하고,
    node->link = g->adj_list[from];
    
    // 2. 헤드(adj_list[from])가 새 노드를 가리키게 합니다.
    // 이렇게 하면 새 노드가 대장(Head)이 됩니다. (PDF 19p 인접리스트 구조) 
    g->adj_list[from] = node;
}

void build_graph(GraphType* g) {
    // 정점의 개수 입력
    int n;
    scanf("%d", &n);

    // 정점의 개수만큼 반복
    char v_label;
    int curr_v, to_v, is_connected;
    for (curr_v = 0; curr_v < n; curr_v++) {
        scanf(" %c", &v_label);
        insert_vertex(g, v_label);

        // 각 정점에 대한 연결 정보 입력
        for (to_v = 0; to_v < n; to_v++) {
            scanf(" %d", &is_connected);

            if (is_connected) insert_edge(g, curr_v, to_v);
        }
    }
}
// 그래프 관련 함수들 끝
//==================================================

//==================================================
// BFS 관련 함수들
int visited[MAX_VERTICES];
void init_visited(GraphType* g) {
    for (int i = 0; i < g->n; i++)
        visited[i] = 0;
}

// [TODO 5] BFS(너비 우선 탐색) 함수
void BFS(GraphType* g, QueueType* q, int v) {
    // 큐에서 꺼낸 정점을 임시로 저장할 변수 (w는 인접한 정점들을 가리킬 포인터)
    ListNode* w;

    // 1. 시작 정점(v)에 '방문함' 도장을 찍습니다. (PDF 36p 알고리즘) [cite: 673-685]
    visited[v] = 1;
    
    // 방문한 정점의 이름을 출력합니다. (예: A)
    printf(" %c", g->vertex_label[v]);
    
    // 시작 정점을 큐(대기줄)에 줄을 세웁니다.
    enqueue(q, v);

    // 2. 큐가 텅 빌 때까지 계속 반복합니다.
    while (!is_empty_queue(q)) {
        // 줄 서 있는 정점 중 맨 앞의 정점을 꺼냅니다.
        v = dequeue(q);

        // 꺼낸 정점(v)의 친구들(인접 정점)을 하나씩 살펴봅니다. (PDF 42p 코드 참조) [cite: 809]
        // w는 연결 리스트를 따라 이동하는 포인터입니다.
        for (w = g->adj_list[v]; w; w = w->link) {
            
            // 만약 그 친구(w->vertex)가 아직 방문하지 않은 곳이라면?
            if (!visited[w->vertex]) {
                // 방문 도장을 꾹! 찍고,
                visited[w->vertex] = 1;
                
                // 이름을 출력하고,
                printf(" %c", g->vertex_label[w->vertex]);
                
                // 다음 순서를 위해 큐에 줄을 세웁니다.
                enqueue(q, w->vertex);
            }
        }
    }
}
// BFS 관련 함수들 끝
//==================================================

int main() {
    QueueType* q;
    GraphType* g;
    q = (QueueType*)malloc(sizeof(QueueType));
    g = (GraphType*)malloc(sizeof(GraphType));

    init_queue(q);
    init_graph(g);

    build_graph(g);

    printf("BFS:");
    init_visited(g);
    BFS(g, q, 0);

    free(g);

    return 0;
}