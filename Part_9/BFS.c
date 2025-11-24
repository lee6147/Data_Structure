#include <stdio.h>
#include <stdlib.h>
//
// ==================================================
// 1. 큐(Queue) 만들기: "방문 대기열"
// 놀이공원에서 놀이기구를 타기 위해 줄을 서는 것과 똑같아요!
// ==================================================
#define MAX_QUEUE_SIZE 20       // 대기열에 최대로 설 수 있는 사람 수
#define INT32_MIN -2147483648   // 큐가 비었을 때 반환할 에러 값 (엄청 작은 수)

typedef struct QueueType {
    int front;                  // 줄의 맨 앞 (나가는 곳)
    int rear;                   // 줄의 맨 뒤 (들어오는 곳)
    int data[MAX_QUEUE_SIZE];   // 실제로 사람들이 서 있는 공간
} QueueType;

// 큐 초기화: "자, 이제 줄 서기 시작합니다!" (아무도 없는 상태)
void init_queue(QueueType* q) {
    q->rear = -1;
    q->front = -1;
}

// 큐가 비었나요?: "줄 서 있는 사람이 한 명도 없나요?"
int is_empty_queue(QueueType* q) { return q->front == q->rear; }

// 큐가 꽉 찼나요?: "더 이상 줄을 설 공간이 없나요?"
int is_full_queue(QueueType* q) { return q->rear == MAX_QUEUE_SIZE - 1; }

// enqueue (삽입): "새로운 손님이 줄의 맨 뒤에 섭니다."
void enqueue(QueueType* q, int item) {
    if (is_full_queue(q)) return; // 꽉 찼으면 못 들어와요.
    q->rear++;                    // 뒷사람 위치를 한 칸 늘리고
    q->data[q->rear] = item;      // 그 자리에 손님(데이터)을 세웁니다.
}

// dequeue (삭제): "맨 앞 손님이 일을 보러 나갑니다."
int dequeue(QueueType* q) {
    if (is_empty_queue(q)) return INT32_MIN; // 비어있으면 나갈 사람이 없어요.
    q->front++;                   // 앞사람 위치를 한 칸 옮겨서
    return q->data[q->front];     // 그 자리에 있던 손님(데이터)을 내보냅니다.
}

// ==================================================
// 2. 그래프(Graph) 만들기: "도시와 도로 지도 그리기"
// 인접 행렬(2차원 표)을 사용해서 연결 관계를 표현합니다.
// ==================================================
#define MAX_VERTICES 256

typedef struct GraphType {
    int n;                           // 도시에 있는 정점(건물)의 개수
    char vertex_label[MAX_VERTICES]; // 정점의 이름표 (예: 'A', 'B', 'C'...)
    int adj_mat[MAX_VERTICES][MAX_VERTICES]; // 연결 지도 (0: 끊김, 1: 연결됨)
} GraphType;

// 방문 체크리스트: "여기는 이미 가봤던 곳인가?"를 기록 (0: 안가봄, 1: 가봄)
int visited[MAX_VERTICES];

// 그래프 초기화: "백지 상태의 지도를 준비합니다."
void init_graph(GraphType* g) {
    int r, c;
    g->n = 0; // 아직 건물(정점)이 하나도 없어요.
    
    // 지도의 모든 연결선을 0(끊김)으로 지워둡니다.
    for (r = 0; r < MAX_VERTICES; r++)
        for (c = 0; c < MAX_VERTICES; c++)
            g->adj_mat[r][c] = 0;
}

// 정점 삽입: "지도에 새로운 건물(A, B, C...)을 등록합니다."
void insert_vertex(GraphType* g, char v_label) {
    // 지도가 꽉 찼으면 더 이상 못 넣어요.
    if (((g->n) + 1) > MAX_VERTICES) {
        fprintf(stderr, "그래프: 정점의 개수 초과");
        return;
    }
    g->vertex_label[g->n] = v_label; // 건물 이름을 기록하고 (예: 0번은 'A')
    g->n++;                          // 건물 개수를 하나 늘립니다.
}

// 간선 삽입: "두 건물 사이에 도로를 깝니다."
void insert_edge(GraphType* g, int from, int to) {
    // 없는 건물 번호면 공사를 못 해요.
    if (from >= g->n || to >= g->n) {
        fprintf(stderr, "그래프: 정점 번호 오류");
        return;
    }
    // 무방향 그래프(양방향 도로)니까 서로 연결해줍니다. (1 = 연결됨)
    g->adj_mat[from][to] = 1; // A에서 B로 가는 길 있음
    g->adj_mat[to][from] = 1; // B에서 A로 가는 길 있음
}

// 방문 체크리스트 초기화: "탐색을 시작하기 전, 아무 데도 안 갔다고 표시합니다."
void init_visited(GraphType* g) {
    for (int i = 0; i < g->n; i++)
        visited[i] = 0; // 0은 False(아직 안 감)를 의미해요.
}

// 그래프 생성 함수: 사용자가 입력한 정보대로 지도를 완성합니다.
void build_graph(GraphType* g) {
    int n;
    scanf("%d", &n); // "건물이 총 몇 개입니까?" (예: 8)

    char v_label;
    int curr_v, to_v, is_connected;

    // 각 건물의 정보를 하나씩 입력받습니다.
    for (curr_v = 0; curr_v < n; curr_v++) {
        scanf(" %c", &v_label); // "이 건물의 이름은 무엇입니까?" (예: A)
        insert_vertex(g, v_label); // 지도에 등록!

        // 다른 건물들과 연결되어 있는지 확인합니다. (예: 0 1 1 0 0...)
        for (to_v = 0; to_v < n; to_v++) {
            scanf("%d", &is_connected);
            if (is_connected) { // "1(연결됨)"이라고 입력받으면
                 // 도로를 깝니다. (행렬에 1 표시)
                 g->adj_mat[curr_v][to_v] = 1; 
            }
        }
    }
}

// ==================================================
// 3. BFS (너비 우선 탐색): "가까운 곳부터 둘러보기"
// 시작점에서 가까운 이웃부터 모두 방문하고, 그 다음 이웃의 이웃으로 넘어갑니다.
// ==================================================
void BFS(GraphType* g, QueueType* q, int v) {
    int w; // 이웃 건물의 번호를 저장할 변수

    visited[v] = 1;          // 1. 출발지에 도착했습니다! "방문 도장 쾅!" (1 = True)
    printf("%c ", g->vertex_label[v]); // 2. 방문한 곳의 이름을 화면에 출력합니다.
    enqueue(q, v);           // 3. "다음에 어디 갈지 정하기 위해" 일단 큐(대기열)에 넣습니다.

    // 대기열(큐)이 텅 빌 때까지 계속 탐색합니다.
    while (!is_empty_queue(q)) {
        v = dequeue(q);      // 4. 대기열의 맨 앞에서 "다음 탐색 기준점"을 하나 꺼냅니다.
        
        // 5. 꺼낸 건물(v)과 연결된 모든 이웃 건물(w)을 살펴봅니다.
        for (w = 0; w < g->n; w++) {
            // (1) v와 w 사이에 도로가 있고(adj_mat == 1),
            // (2) w에 아직 가본 적이 없다면(!visited)
            if (g->adj_mat[v][w] && !visited[w]) {
                visited[w] = 1;    // "여기 방문합니다!" 도장 찍고
                printf("%c ", g->vertex_label[w]); // 이름 출력하고
                enqueue(q, w);     // "이 친구의 이웃도 나중에 가봐야 하니 대기열에 줄 서세요."
            }
        }
    }
}

// ==================================================
// 4. 메인 함수: 프로그램이 시작되는 곳
// ==================================================
int main() {
    QueueType* q;   // 큐를 가리킬 포인터
    GraphType* g;   // 그래프를 가리킬 포인터

    // 메모리 공간을 빌려옵니다 (동적 할당)
    q = (QueueType*)malloc(sizeof(QueueType));
    g = (GraphType*)malloc(sizeof(GraphType));

    // 큐와 그래프를 깨끗하게 초기화합니다.
    init_queue(q);
    init_graph(g);

    // 사용자의 입력을 받아 지도를 완성합니다.
    build_graph(g);

    // BFS 탐색 시작!
    printf("BFS: ");
    init_visited(g); // 방문 기록을 깨끗이 지우고
    BFS(g, q, 0);    // 0번 건물(첫 번째 입력된 건물)부터 탐색을 시작합니다.

    // 빌려온 메모리를 반납합니다.
    free(q);
    free(g);

    return 0;
}