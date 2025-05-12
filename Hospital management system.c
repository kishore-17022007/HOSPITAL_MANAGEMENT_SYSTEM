#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 5
#define MAX_DOCTORS 10
#define MAX_APPOINTMENTS 20

typedef struct Patient {
    int id;
    char name[50];
    int severity;
} Patient;

typedef struct Doctor {
    int id;
    char name[50];
    int available; 
} Doctor;

typedef struct Appointment {
    Patient patient;
    Doctor doctor;
    char time[10]; 
} Appointment;


Patient queue[SIZE];
int front = -1, rear = -1;

Doctor doctors[MAX_DOCTORS];
int doctorCount = 0;

Appointment appointments[MAX_APPOINTMENTS];
int appointmentCount = 0;

int isFull() {
    return (front == (rear + 1) % SIZE);
}

int isEmpty() {
    return front == -1;
}

void enqueue(Patient p) {
    if (isFull()) {
        printf("Queue is full. Cannot add patient.\n");
        return;
    }
    if (isEmpty()) front = 0;
    rear = (rear + 1) % SIZE;
    queue[rear] = p;
    printf("Patient enqueued successfully.\n");
}

Patient dequeue() {
    Patient p = {-1, "", -1};
    if (isEmpty()) {
        printf("Queue is empty.\n");
        return p;
    }
    p = queue[front];
    if (front == rear) front = rear = -1;
    else front = (front + 1) % SIZE;
    return p;
}

typedef struct BSTNode {
    Patient patient;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

BSTNode* insert(BSTNode* root, Patient p) {
    if (root == NULL) {
        BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
        newNode->patient = p;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (p.severity < root->patient.severity) root->left = insert(root->left, p);
    else root->right = insert(root->right, p);
    return root;
}

void inorder(BSTNode* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("ID: %d, Name: %s, Severity: %d\n", root->patient.id, root->patient.name, root->patient.severity);
        inorder(root->right);
    }
}

Patient* search(BSTNode* root, int id) {
    if (root == NULL) return NULL;
    if (root->patient.id == id) return &root->patient;
    Patient* leftResult = search(root->left, id);
    if (leftResult != NULL) return leftResult;
    return search(root->right, id);
}

void freeBST(BSTNode* root) {
    if (root == NULL) return;
    freeBST(root->left);
    freeBST(root->right);
    free(root);
}

void addDoctor() {
    if (doctorCount >= MAX_DOCTORS) {
        printf("Doctor limit reached.\n");
        return;
    }
    Doctor d;
    printf("Enter Doctor ID: ");
    scanf("%d", &d.id); getchar();
    printf("Enter Doctor Name: ");
    fgets(d.name, sizeof(d.name), stdin);
    d.name[strcspn(d.name, "\n")] = 0;
    d.available = 1;
    doctors[doctorCount++] = d;
    printf("Doctor added successfully.\n");
}

void showAvailableDoctors() {
    printf("\n--- Available Doctors ---\n");
    int found = 0;
    for (int i = 0; i < doctorCount; i++) {
        if (doctors[i].available) {
            printf("ID: %d, Name: %s\n", doctors[i].id, doctors[i].name);
            found = 1;
        }
    }
    if (!found) printf("No doctors available right now.\n");
}

Doctor* getDoctorById(int id) {
    for (int i = 0; i < doctorCount; i++) {
        if (doctors[i].id == id) return &doctors[i];
    }
    return NULL;
}

void bookAppointment() {
    if (isEmpty()) {
        printf("No patients in queue to book appointment.\n");
        return;
    }
    Patient p = dequeue();
    printf("Patient %s dequeued.\n", p.name);

    showAvailableDoctors();
    printf("Enter Doctor ID to assign: ");
    int docId;
    scanf("%d", &docId); getchar();

    Doctor* d = getDoctorById(docId);
    if (d == NULL || !d->available) {
        printf("Invalid or unavailable doctor.\n");
        return;
    }

    Appointment a;
    a.patient = p;
    a.doctor = *d;

    printf("Enter appointment time (e.g., 10:30AM): ");
    fgets(a.time, sizeof(a.time), stdin);
    a.time[strcspn(a.time, "\n")] = 0;

    appointments[appointmentCount++] = a;
    d->available = 0;

    printf("Appointment booked for %s with Dr. %s at %s\n", p.name, d->name, a.time);
}

void showAppointments() {
    printf("\n--- Appointments List ---\n");
    if (appointmentCount == 0) {
        printf("No appointments booked.\n");
        return;
    }
    for (int i = 0; i < appointmentCount; i++) {
        printf("Patient: %s (ID: %d), Doctor: %s, Time: %s\n",
            appointments[i].patient.name,
            appointments[i].patient.id,
            appointments[i].doctor.name,
            appointments[i].time);
    }
}

int main() {
    BSTNode* bstRoot = NULL;
    int choice;
    Patient temp;
    int searchId;

    do {
        printf("\n--- Hospital Patient Management System ---\n");
        printf("1. Add Doctor\n");
        printf("2. Show Available Doctors\n");
        printf("3. Book Appointment\n");
        printf("4. Show Appointments\n");
        printf("5. Add patient to queue\n");
        printf("6. Process next patient (move to BST)\n");
        printf("7. Display patient records (Inorder - by severity)\n");
        printf("8. Search patient by ID\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            break;
        }
        getchar(); 

        switch (choice) {
            case 1:
                addDoctor();
                break;

            case 2:
                showAvailableDoctors();
                break;

            case 3:
                bookAppointment();
                break;

            case 4:
                showAppointments();
                break;

            case 5:
                printf("Enter Patient ID: ");
                if (scanf("%d", &temp.id) != 1) {
                    printf("Invalid input.\n");
                    break;
                }
                getchar();
                printf("Enter Patient Name: ");
                if (fgets(temp.name, sizeof(temp.name), stdin) == NULL) {
                    printf("Error reading name.\n");
                    break;
                }
                temp.name[strcspn(temp.name, "\n")] = 0;
                printf("Enter Severity (1-10): ");
                if (scanf("%d", &temp.severity) != 1 || temp.severity < 1 || temp.severity > 10) {
                    printf("Invalid severity. Must be 1 to 10.\n");
                    break;
                }
                getchar();
                enqueue(temp);
                break;

            case 6:
                if (!isEmpty()) {
                    temp = dequeue();
                    printf("Processing Patient: ID=%d, Name=%s, Severity=%d\n", temp.id, temp.name, temp.severity);
                    bstRoot = insert(bstRoot, temp);
                } else {
                    printf("Queue is empty.\n");
                }
                break;

            case 7:
                printf("\n--- Patient Records (Sorted by Severity) ---\n");
                if (bstRoot == NULL) printf("No records found.\n");
                else inorder(bstRoot);
                break;

            case 8:
                printf("Enter Patient ID to search: ");
                if (scanf("%d", &searchId) != 1) {
                    printf("Invalid input.\n");
                    break;
                }
                getchar();
                Patient* found = search(bstRoot, searchId);
                if (found) {
                    printf("Patient Found: ID=%d, Name=%s, Severity=%d\n", found->id, found->name, found->severity);
                } else {
                    printf("Patient with ID %d not found.\n", searchId);
                }
                break;

            case 9:
                printf("Exiting program...\n");
                freeBST(bstRoot);
                break;

            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 9);

    return 0;
}
