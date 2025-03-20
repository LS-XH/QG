#define _CRT_SECURE_NO_WARNINGS 0
#include <stdio.h>
#include <stdlib.h>


//单向链表点
typedef struct SinglyListNode {
    int val;
    struct SinglyListNode* next;
} SinglyListNode;
//单向链表
typedef struct SinglyLinkedList {
    SinglyListNode* head;
    SinglyListNode* tail;
} SinglyLinkedList;
//双向链表点
typedef struct DoublyListNode {
    int val;
    struct DoublyListNode* prev;
    struct DoublyListNode* next;
} DoublyListNode;
//双向链表
typedef struct DoublyLinkedList {
    DoublyListNode* head;
    DoublyListNode* tail;
} DoublyLinkedList;

//单链表操作
void sll_init(SinglyLinkedList* list);
void sll_destroy(SinglyLinkedList* list);
void sll_insert_at_tail(SinglyLinkedList* list, int val);
void sll_print(const SinglyLinkedList* list);
void sll_swap_pairs(SinglyLinkedList* list);
SinglyListNode* sll_find_middle(const SinglyLinkedList* list);
int sll_has_cycle(const SinglyLinkedList* list);
void sll_reverse_iterative(SinglyLinkedList* list);
void sll_reverse_recursive(SinglyLinkedList* list);
void sll_create_cycle(SinglyLinkedList* list, int pos);

//双向链表操作
void dll_init(DoublyLinkedList* list);
void dll_destroy(DoublyLinkedList* list);
void dll_insert_at_tail(DoublyLinkedList* list, int val);
void dll_print(const DoublyLinkedList* list);






//检测并断裂环
static int detect_and_break_cycle(SinglyLinkedList* list) {
    if (!list->head || !list->head->next) return 0;

    SinglyListNode* slow = list->head, * fast = list->head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) break;
    }

    if (slow != fast) return 0;

    slow = list->head;
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }

    SinglyListNode* cycle_node = slow;
    if (cycle_node == list->head) {
        SinglyListNode* p = cycle_node;
        while (p->next != cycle_node) p = p->next;
        p->next = NULL;
    }
    else {
        SinglyListNode* prev = list->head;
        while (prev->next != cycle_node) prev = prev->next;
        prev->next = NULL;
    }
    return 1;
}

//初始化链表
void sll_init(SinglyLinkedList* list) {
    list->head = list->tail = NULL;
}
//删除链表
void sll_destroy(SinglyLinkedList* list) {
    detect_and_break_cycle(list);
    SinglyListNode* curr = list->head;
    while (curr) {
        SinglyListNode* next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = list->tail = NULL;
}
//尾部插入
void sll_insert_at_tail(SinglyLinkedList* list, int val) {
    SinglyListNode* node = (SinglyListNode*)malloc(sizeof(SinglyListNode));
    node->val = val;
    node->next = NULL;

    if (!list->head) {
        list->head = list->tail = node;
    }
    else {
        list->tail->next = node;
        list->tail = node;
    }
}
//遍历打印
void sll_print(const SinglyLinkedList* list) {
    SinglyListNode* curr = list->head;
    while (curr) {
        printf("%d", curr->val);
        if (curr->next) printf(" -> ");
        curr = curr->next;
    }
    printf("\n");
}
//奇偶调换
void sll_swap_pairs(SinglyLinkedList* list) {
    SinglyListNode dummy = { 0, list->head };
    SinglyListNode* curr = &dummy;

    while (curr->next && curr->next->next) {
        SinglyListNode* first = curr->next;
        SinglyListNode* second = first->next;

        curr->next = second;
        first->next = second->next;
        second->next = first;

        curr = first;
    }

    list->head = dummy.next;
    list->tail = list->head;
    while (list->tail && list->tail->next) {
        list->tail = list->tail->next;
    }
}
//寻找中点
SinglyListNode* sll_find_middle(const SinglyLinkedList* list) {
    if (!list->head) return NULL;
    SinglyListNode* slow = list->head, * fast = list->head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}
//判断环
int sll_has_cycle(const SinglyLinkedList* list) {
    if (!list->head) return 0;
    SinglyListNode* slow = list->head, * fast = list->head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return 1;
    }
    return 0;
}
//反转-迭代
void sll_reverse_iterative(SinglyLinkedList* list) {
    SinglyListNode* prev = NULL, * curr = list->head;
    list->tail = list->head;
    while (curr) {
        SinglyListNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    list->head = prev;
}
//用于反向递归
static SinglyListNode* reverse_recursive_helper(SinglyListNode* head) {
    if (!head || !head->next) return head;
    SinglyListNode* new_head = reverse_recursive_helper(head->next);
    head->next->next = head;
    head->next = NULL;
    return new_head;
}
//反转-递归
void sll_reverse_recursive(SinglyLinkedList* list) {
    list->head = reverse_recursive_helper(list->head);
    list->tail = list->head;
    while (list->tail && list->tail->next) {
        list->tail = list->tail->next;
    }
}
//创建环
void sll_create_cycle(SinglyLinkedList* list, int pos) {
    if (pos < 0) return;
    SinglyListNode* curr = list->head;
    SinglyListNode* target = NULL;
    int index = 0;
    while (curr) {
        if (index == pos) target = curr;
        if (!curr->next) {
            curr->next = target;
            return;
        }
        curr = curr->next;
        index++;
    }
}





//双向链表
void dll_init(DoublyLinkedList* list) {
    list->head = list->tail = NULL;
}
//删除链表
void dll_destroy(DoublyLinkedList* list) {
    DoublyListNode* curr = list->head;
    while (curr) {
        DoublyListNode* next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = list->tail = NULL;
}
//尾部插入
void dll_insert_at_tail(DoublyLinkedList* list, int val) {
    DoublyListNode* node = (DoublyListNode*)malloc(sizeof(DoublyListNode));
    node->val = val;
    node->prev = list->tail;
    node->next = NULL;

    if (!list->head) {
        list->head = list->tail = node;
    }
    else {
        list->tail->next = node;
        list->tail = node;
    }
}
//遍历打印
void dll_print(const DoublyLinkedList* list) {
    DoublyListNode* curr = list->head;
    while (curr) {
        printf("%d", curr->val);
        if (curr->next) printf(" <-> ");
        curr = curr->next;
    }
    printf("\n");
}

//用于清除输入的\n
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
//单链表主函数
void handle_singly_linked_list() {
    SinglyLinkedList sll;
    sll_init(&sll);

    int choice;
    while (1) {
        printf("\n单链表操作菜单:\n"
            "1. 插入节点\n"
            "2. 奇偶调换\n"
            "3. 查找中点\n"
            "4. 检测环\n"
            "5. 反转链表(迭代)\n"
            "6. 反转链表(递归)\n"
            "7. 创建环\n"
            "8. 打印链表\n"
            "0. 返回主菜单\n"
            "请选择操作: ");

        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }

        switch (choice) {
        case 1: {
            int val;
            printf("输入节点值(-1结束): ");
            while (scanf("%d", &val) == 1 && val != -1) {
                sll_insert_at_tail(&sll, val);
                printf("继续输入: ");
            }
            clear_input_buffer();
            break;
        }
        case 2:
            sll_swap_pairs(&sll);
            printf("操作后链表: ");
            sll_print(&sll);
            break;
        case 3: {
            SinglyListNode* mid = sll_find_middle(&sll);
            if (mid) printf("中间节点值: %d\n", mid->val);
            else printf("空链表\n");
            break;
        }
        case 4:
            printf(sll_has_cycle(&sll) ? "存在环\n" : "无环\n");
            break;
        case 5:
            sll_reverse_iterative(&sll);
            printf("反转后: ");
            sll_print(&sll);
            break;
        case 6:
            sll_reverse_recursive(&sll);
            printf("反转后: ");
            sll_print(&sll);
            break;
        case 7: {
            int pos;
            printf("输入环位置: ");
            if (scanf("%d", &pos) == 1) sll_create_cycle(&sll, pos);
            clear_input_buffer();
            break;
        }
        case 8:
            sll_print(&sll);
            break;
        case 0:
            sll_destroy(&sll);
            return;
        default:
            printf("无效输入\n");
        }
    }
}
//双链表主函数
void handle_doubly_linked_list() {
    DoublyLinkedList dll;
    dll_init(&dll);

    int choice;
    while (1) {
        printf("\n双向链表操作菜单:\n"
            "1. 插入节点\n"
            "2. 打印链表\n"
            "0. 返回主菜单\n"
            "请选择操作: ");

        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }

        switch (choice) {
        case 1: {
            int val;
            printf("输入节点值(-1结束): ");
            while (scanf("%d", &val) == 1 && val != -1) {
                dll_insert_at_tail(&dll, val);
                printf("继续输入: ");
            }
            clear_input_buffer();
            break;
        }
        case 2:
            dll_print(&dll);
            break;
        case 0:
            dll_destroy(&dll);
            return;
        default:
            printf("无效输入\n");
        }
    }
}

int main() {
    int choice;
    do {
        printf("\n主菜单:\n"
            "1. 单链表操作\n"
            "2. 双向链表操作\n"
            "0. 退出\n"
            "请选择: ");

        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }

        switch (choice) {
        case 1:
            handle_singly_linked_list();
            break;
        case 2:
            handle_doubly_linked_list();
            break;
        case 0:
            printf("程序退出\n");
            break;
        default:
            printf("无效选择\n");
        }
    } while (choice != 0);

    return 0;
}