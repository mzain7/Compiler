#include <iostream>
#include <string>
using namespace std;

class Rule {
public:
    string terms[10];
    int num_terms;
    Rule* next;
    Rule* prev;

    Rule(string termArray[], int n){
        num_terms = n;
        next = nullptr;
        prev = nullptr;
        for (int i = 0; i < n; ++i) {
            terms[i] = termArray[i];
        }
    }

    void display() {
        for (int i = 0; i < num_terms; ++i) {
            cout << terms[i];
            if (i != num_terms - 1) cout << " ";
        }
    }
};

class Prod {
public:
    string name;
    Prod* next;
    Prod* prev;
    Rule* first_rule;
    Rule* last_rule;

    Prod(string prodName) {
        name = prodName;
        next = nullptr;
        prev = nullptr;
        first_rule = nullptr;
        last_rule = nullptr;
    }

    void addRule(Rule* rule) {
        if (!first_rule) {
            first_rule = rule;
            last_rule = rule;
        } else {
            last_rule->next = rule;
            rule->prev = last_rule;
            last_rule = rule;
        }
    }

    void display() {
        cout << name << " -> ";
        Rule* current_rule = first_rule;
        while (current_rule) {
            current_rule->display();
            if (current_rule->next) cout << " | ";
            current_rule = current_rule->next;
        }
        cout << endl;
    }
};

// The DoublyLinkedList stores productions in a doubly linked list, with each Prod pointing to the next and previous productions. This allows for easy traversal between different productions, either forwards or backwards.
class DoublyLinkedList {
public:
    Prod* head;
    Prod* tail;

    DoublyLinkedList() {
        head = nullptr;
        tail = nullptr;
    }

    void addProd(Prod* prod) {
        if (!head) {
            head = prod;
            tail = prod;
        } else {
            tail->next = prod;
            prod->prev = tail;
            tail = prod;
        }
    }

    void displayForward() {
        Prod* current_prod = head;
        while (current_prod) {
            current_prod->display();
            current_prod = current_prod->next;
        }
    }

    void displayBackward() {
        Prod* current_prod = tail;
        while (current_prod) {
            current_prod->display();
            current_prod = current_prod->prev;
        }
    }

    ~DoublyLinkedList() {
        while (head) {
            Prod* current_prod = head;
            head = head->next;
            Rule* current_rule = current_prod->first_rule;
            while (current_rule) {
                Rule* next_rule = current_rule->next;
                delete current_rule;
                current_rule = next_rule;
            }
            delete current_prod;
        }
    }
};


int main(){
    DoublyLinkedList grammer;

    while(true){
        printf("Enter");

        string line;
        getline(cin, line);

        if(line =="end"){
            break;
        }

        
    }
    return 0;
}