#include &lt;stdint.h&gt;
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;

// [큐(Queue) 설정]
// 큐는 '놀이공원 줄 서기'와 같아요. 먼저 온 사람이 먼저 나갑니다 (First-In-First-Out).
#define MAX_QUEUE_SIZE 20 // 줄 설 수 있는 최대 인원은 20명으로 제한

typedef struct QueueType {
    int front; // 줄의 맨 앞사람 위치 (나가는 곳)
    int rear;  // 줄의 맨 뒷사람 위치 (들어오는 곳)
    int data[MAX_QUEUE_SIZE]; // 실제로 사람들이 서 있는 공간
} QueueType;

//==================================================
// 큐(줄 서기) 관련 함수들

// 1. 큐 초기화: 줄을 텅 비게 만듭니다.
void init_queue(QueueType* q) {
    q-&gt;rear = -1;
    q-&gt;front = -1;
}

// 2. 큐가 비었는지 확인: 앞(front)과 뒤(rear)가 같으면 아무도 없는 거예요.
int is_empty_queue(QueueType* q) { 
    return q-&gt;front == q-&gt;rear; 
}

// 3. 큐가 꽉 찼는지 확인: 뒤(rear)가 끝까지 갔으면 더 못 들어와요.
int is_full_queue(QueueType* q) { 
    return q-&gt;rear == MAX_QUEUE_SIZE - 1; 
}

// 4. 데이터 넣기 (Enqueue): 줄의 맨 뒤에 사람을 세웁니다.
void enqueue(QueueType* q, int item) {
    if (is_full_queue(q)) return; // 꽉 찼으면 돌려보냄
    q-&gt;rear++;              // 뒷사람 위치를 한 칸 뒤로 밀고
    q-&gt;data[q-&gt;rear] = item; // 그 자리에 데이터를 넣음
}

// 5. 데이터 꺼내기 (Dequeue): 줄의 맨 앞사람을 내보냅니다.
int dequeue(QueueType* q) {
    if (is_empty_queue(q)) return INT32_MIN; // 비어있으면 에러 값 반환
    q-&gt;front++;               // 앞사람 위치를 한 칸 뒤로 밀고 (나갔으니)
    return q-&gt;data[q-&gt;front]; // 나간 사람의 데이터를 반환
}

// 큐 관련 함수들 끝
//==================================================


// [그래프 설정]
#define MAX_VERTICES 256 // 도시(정점)는 최대 256개까지 가능

// [연결 리스트의 노드: 쪽지]
// "여기서 다음엔 저기로 가세요"라고 적힌 쪽지입니다.
typedef struct ListNode {
    int vertex;            // 목적지 도시 번호
    struct ListNode* link; // 다음 쪽지를 가리키는 밧줄(포인터)
} ListNode;

// [그래프 구조체: 지도 대장]
typedef struct GraphType {
    int n;                            // 현재 등록된 도시의 수
    char vertex_label[MAX_VERTICES];  // 도시의 실제 이름 (A, B, C...)
    ListNode* adj_list[MAX_VERTICES]; // 각 도시별 '연결된 이웃 목록'의 시작점
} GraphType;

//==================================================
// 그래프 관련 함수들

// 그래프가 텅 비었는지 확인
int is_empty(GraphType* g) { 
    return (g-&gt;n == 0); 
}

// 그래프가 꽉 찼는지 확인
int is_full(GraphType* g) { 
    return (g-&gt;n &gt;= MAX_VERTICES); 
}

// 1. 그래프 초기화: 백지상태로 만듭니다.
void init_graph(GraphType* g) {
    /* [ 구현 부분 시작 ] */
    g-&gt;n = 0; // 도시 0개
    // 모든 도시의 이웃 목록을 '없음(NULL)'으로 초기화
    for (int i = 0; i &lt; MAX_VERTICES; i++) {
        g-&gt;adj_list[i] = NULL;
    }
    /* [ 구현 부분 끝 ] */
}

// 2. 그래프 삭제: 빌려 쓴 메모리(쪽지들)를 다 정리합니다.
void free_graph(GraphType* g) {
    /* [ 구현 부분 시작 ] */
    for (int i = 0; i &lt; g-&gt;n; i++) {
        ListNode* p = g-&gt;adj_list[i]; // i번째 도시의 이웃 목록을 가져와서
        while (p != NULL) {           // 목록이 끝날 때까지
            ListNode* temp = p;       // 현재 쪽지를 잡고
            p = p-&gt;link;              // 다음 쪽지로 이동한 뒤
            free(temp);               // 잡은 쪽지는 폐기(반납)
        }
    }
    /* [ 구현 부분 끝 ] */
}

// 3. 정점(도시) 등록: 지도에 도시 이름을 하나 추가합니다.
void insert_vertex(GraphType* g, char v_label) {
    /* [ 구현 부분 시작 ] */
    if (is_full(g)) return;          // 더 못 그리면 포기
    g-&gt;vertex_label[g-&gt;n] = v_label; // n번째 칸에 이름(A, B..) 적기
    g-&gt;n++;                          // 도시 개수 1 증가
    /* [ 구현 부분 끝 ] */
}

// 4. 간선(길) 연결: 두 도시 사이의 길을 등록합니다.
// 중요: 이웃 목록의 '맨 앞'에 새치기하듯 끼워 넣습니다.
// (PDF의 Head Insertion 방식: 나중에 들어온 게 리스트의 앞에 옴)
void insert_edge(GraphType* g, int from, int to) {
    /* [ 구현 부분 시작 ] */
    // 1. 새 쪽지(노드)를 하나 만들고 메모리를 빌려요.
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    
    // 2. 쪽지에 목적지(to)를 적어요.
    node-&gt;vertex = to;
    
    // 3. 이 쪽지가 원래 맨 앞에 있던 쪽지를 가리키게 해요.
    node-&gt;link = g-&gt;adj_list[from];
    
    // 4. 이제 이 쪽지가 리스트의 '새로운 맨 앞'이 됩니다.
    g-&gt;adj_list[from] = node;
    /* [ 구현 부분 끝 ] */
}

// 입력값을 받아서 그래프를 완성하는 함수
void build_graph(GraphType* g) {
    int n;
    scanf("%d", &amp;n); // 도시 개수 입력
    
    char v_label;
    int curr_v, to_v, is_connected;
    for (curr_v = 0; curr_v &lt; n; curr_v++) {
        scanf(" %c", &amp;v_label);     // 도시 이름 입력
        insert_vertex(g, v_label);  // 도시 등록
        
        // 연결 정보 입력 (0 또는 1)
        for (to_v = 0; to_v &lt; n; to_v++) {
            scanf(" %d", &amp;is_connected);
            // 1이면 연결된 것이므로 길을 추가해요.
            if (is_connected) insert_edge(g, curr_v, to_v);
        }
    }
}

// 그래프 관련 함수들 끝
//==================================================


//==================================================
// BFS (너비 우선 탐색) 관련 함수들

// 방문 체크 리스트 (0: 안 가봄, 1: 가봄)
int visited[MAX_VERTICES];

// 방문 기록을 모두 지워서 초기화
void init_visited(GraphType* g) {
    for (int i = 0; i &lt; g-&gt;n; i++)
        visited[i] = 0;
}

// ★ BFS 핵심 알고리즘 ★
// 시작점에서 가까운 곳부터 차례대로 방문합니다. (큐 사용)
void BFS(GraphType* g, QueueType* q, int v) {
    /* [ 구현 부분 시작 ] */
    ListNode* w; // 이웃을 가리킬 포인터
    
    // 1. 시작 단계
    visited[v] = 1;                    // 시작 도시 방문 체크!
    printf("%c ", g-&gt;vertex_label[v]); // 도시 이름 출력 (예: A)
    enqueue(q, v);                     // 큐(대기열)에 시작 도시 등록
    
    // 2. 반복 단계 (큐가 빌 때까지)
    while (!is_empty_queue(q)) {
        v = dequeue(q); // 큐 맨 앞에서 도시 하나를 꺼내옵니다.
        
        // 꺼내온 도시(v)의 이웃들을 쭉 살펴봅니다.
        // g-&gt;adj_list[v] (첫 이웃)부터 시작해서 link가 없을 때까지 반복
        for (w = g-&gt;adj_list[v]; w; w = w-&gt;link) {
            
            // 만약 그 이웃(w-&gt;vertex)을 아직 방문 안 했다면?
            if (!visited[w-&gt;vertex]) {
                visited[w-&gt;vertex] = 1; // 방문 도장 쾅!
                printf("%c ", g-&gt;vertex_label[w-&gt;vertex]); // 이름 출력
                enqueue(q, w-&gt;vertex);  // 다음에 그 친구의 이웃도 봐야 하니까 큐에 줄 세움
            }
        }
    }
    /* [ 구현 부분 끝 ] */
}

// BFS 관련 함수들 끝
//==================================================


int main() {
    // 큐와 그래프를 위한 메모리 공간(방)을 빌립니다.
    QueueType* q = (QueueType*)malloc(sizeof(QueueType));
    GraphType* g = (GraphType*)malloc(sizeof(GraphType));
    
    // 큐와 그래프 초기화
    init_queue(q);
    init_graph(g);
    
    // 입력을 받아서 그래프(지도) 완성
    build_graph(g);
    
    // BFS 탐색 시작
    // **주의**: 출력 형식 "BFS: " (공백 포함)을 맞춰야 테스트를 통과합니다.
    printf("BFS: ");
    
    init_visited(g); // 방문 기록 초기화
    BFS(g, q, 0);    // 0번 도시부터 탐색 시작!
    
    // 다 썼으니 빌린 메모리 반납 (청소)
    free_graph(g); // 리스트 노드들 해제
    free(g);       // 그래프 구조체 해제
    free(q);       // 큐 구조체 해제
    
    return 0;
}