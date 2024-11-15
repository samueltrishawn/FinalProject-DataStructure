#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Customer {
    int id;
    char name[50];
    char address[100];
    char mobile_number[13]; 
} Customer;

typedef struct Node {
    Customer customer;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

int height(Node* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node* createNode(Customer customer) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->customer = customer;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(Node* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node* insertNode(Node* node, Customer customer) {
    if (node == NULL)
        return createNode(customer);
    if (customer.id < node->customer.id)
        node->left = insertNode(node->left, customer);
    else if (customer.id > node->customer.id)
        node->right = insertNode(node->right, customer);
    else {
        printf("Customer with ID %d already exists. Ignoring duplicate entry.\n", customer.id);
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);
    if (balance > 1 && customer.id < node->left->customer.id)
        return rightRotate(node);
    if (balance < -1 && customer.id > node->right->customer.id)
        return leftRotate(node);
    if (balance > 1 && customer.id > node->left->customer.id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && customer.id < node->right->customer.id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Node* deleteNode(Node* root, int id) {
    if (root == NULL) {
        printf("There are no customers yet.\n");
        return root;
    }

    Node* temp;
    if (id < root->customer.id)
        root->left = deleteNode(root->left, id);
    else if (id > root->customer.id)
        root->right = deleteNode(root->right, id);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
            printf("Customer with ID %d has been successfully deleted.\n", id);
        } else {
            temp = minValueNode(root->right);
            root->customer.id = temp->customer.id;
            root->right = deleteNode(root->right, temp->customer.id);
        }
    }

    if (root == NULL)
        return root;
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

Node* searchNode(Node* root, int id) {
    if (root == NULL || root->customer.id == id)
        return root;
    if (root->customer.id < id)
        return searchNode(root->right, id);
    return searchNode(root->left, id);
}

void printPreOrder(Node* root) {
    if (root != NULL) {
        printf("Customer ID: %d, Name: %s, Address: %s, Mobile Number: %s\n", root->customer.id, root->customer.name, root->customer.address, root->customer.mobile_number);
        printPreOrder(root->left);
        printPreOrder(root->right);
    }
}

void inOrder(Node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("Customer ID: %d, Name: %s, Address: %s, Mobile Number: %s\n", root->customer.id, root->customer.name, root->customer.address, root->customer.mobile_number);
        inOrder(root->right);
    }
}

void displayCustomersInOrder(Node* root) {
    if (root == NULL) {
        printf("There are no customers yet.\n");
        return;
    }

    printf("Stored Customer data in AVL tree (In-Order):\n");
    printf("---------------------------------\n");
    inOrder(root);
    printf("---------------------------------\n");
}

void displayCustomersPreOrder(Node* root) {
    if (root == NULL) {
        printf("There are no customers yet.\n");
        return;
    }

    printf("Stored Customer data in AVL tree (Pre-Order):\n");
    printf("---------------------------------\n");
    printPreOrder(root);
    printf("---------------------------------\n");
}

int isAlphabetic(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isalpha(str[i]))
            return 0;
    }
    return 1;
}

int main() {
    Node* root = NULL;
    Node* temp = NULL;
    int id;
    char name[50];
    char address[100];
    char mobile_number[13];
    char option;

    while (1) {
    	system("cls");
        printf("Menu:\n");
        printf("1. Insert customer data\n");
        printf("2. Display all customer data (In-Order)\n");
        printf("3. Display all customer data (Pre-Order)\n");
        printf("4. Search for a customer\n");
        printf("5. Delete a customer\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf(" %c", &option);

        switch (option) {
            case '1':
                printf("Enter customer ID: ");
                scanf("%d", &id);
                printf("Enter customer name: ");
                scanf(" %[^\n]s", name);

                if (!isAlphabetic(name)) {
                    printf("Invalid name. Please enter alphabetic characters only.\n");
					system("pause");
					break;
                }

                printf("Enter customer address: ");
                scanf(" %[^\n]s", address);
                printf("Enter customer mobile number (starting with '08', 11-12 digits): ");
                scanf(" %[^\n]s", mobile_number);

                if (strlen(mobile_number) < 11 || strlen(mobile_number) > 12 || strncmp(mobile_number, "08", 2) != 0) {
                    printf("Invalid mobile number. Please enter a mobile number starting with '08' and with 11-12 digits.\n");
                    system("pause");
					break;
                }

                Customer customer = {id, "", "", ""};
                strcpy(customer.name, name);
                strcpy(customer.address, address);
                strcpy(customer.mobile_number, mobile_number);

                root = insertNode(root, customer);
                system("pause");
				break;

            case '2':
                displayCustomersInOrder(root);
                system("pause");
				break;

            case '3':
                displayCustomersPreOrder(root);
                system("pause");
				break;

            case '4':
                printf("\nEnter ID of customer to search: ");
                scanf("%d", &id);
                temp = searchNode(root, id);
                if (temp != NULL)
                    printf("\nFound customer with ID: %d, Name: %s, Address: %s, Mobile Number: %s\n", temp->customer.id, temp->customer.name, temp->customer.address, temp->customer.mobile_number);
                else
                    printf("\nCustomer not found.\n");
                system("pause");
				break;

            case '5':
                printf("\nEnter ID of customer to delete: ");
                scanf("%d", &id);
                root = deleteNode(root, id);
                system("pause");
				break;

            case '6':
                printf("\nExiting...\n");
                exit(0);

            default:
                printf("\nInvalid choice. Please enter a valid option.\n");
        }
    }

    return 0;
}

