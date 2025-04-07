#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_SEATS 16
#define V 6
#define row 4

int route=0;
int chooseRoute;
int rprice[MAX_SEATS];
int busSeats[row][row];
int originalBusSeats[row][row];

// Define a stack structure for seat reservation
typedef struct {
    int data[MAX_SEATS];
    int top;
} Stack;

// Passengers Info
typedef struct {
    char name[50];
    int age;
    int seats[MAX_SEATS];
    int seatCount;
} Reservation;

// Initialize an empty stack
void initializeStack(Stack* stack) {
    stack->top = -1;
}

void initializerprice(int rprice[]){
    for(int i=0;i<MAX_SEATS;i++)
    {
        rprice[i]=0;
    }
}
void initializeMatrix(int busSeats[][row])
{
    int k=1;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<row;j++)
        {
            busSeats[i][j]=k;
            originalBusSeats[i][j] = k;
            k++;
        }
    }
}

// Check if the stack is empty
int isStackEmpty(const Stack* stack) {
    return stack->top == -1;
}

// Check if the stack is full
int isStackFull(const Stack* stack) {
    return stack->top == MAX_SEATS - 1;
}

// Push an element onto the stack
void push(Stack* stack, int seat) {
    if (!isStackFull(stack)) {
        stack->data[++stack->top] = seat;
    } else {
        printf("Stack is full. Cannot add more seats.\n");
    }
}

// Pop an element from the stack
int pop(Stack* stack) {
    if (!isStackEmpty(stack)) {
        return stack->data[stack->top--];
    } else {
        printf("Stack is empty. No seats to cancel.\n");
        return -1;
    }
}

void bookSeat(int busSeats[][row],int bookt,Stack* stack)
{
    int found = 0;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<row;j++)
        {
            if(busSeats[i][j]==bookt && busSeats[i][j]!=0)
            {
                busSeats[i][j]=0;
                found=1;
                pop(stack);
            }
        }
    }
    if (found == 0) {
        printf("Seat is not available.\n");
    }

}

// Display the current seat availability in a bus format
void displaySeatAvailability(const int busSeats[][row]) {
    printf("Available seats in bus format:\n");
    for (int i = 0; i < row ; i++) {
        for(int j=0;j<row ;j++){
            printf("%d ",busSeats[i][j]);
        }
        printf("\n");
    }
    
}

// Get passenger information
void getPassengerInfo(Reservation* reservation) {
    printf("\n\n(Under Name you want the booking to be?)\n");
    printf("(User should be above 18!)\n");
    printf("Enter your name: ");
    scanf(" %[^\n]s", reservation->name);
    printf("Enter your age: ");
    scanf("%d", &reservation->age);
    if((reservation->age)<18)
    {
        printf("User must be 18 and above to reserve a ticket.\n");
        exit(0);
    }
}

// Display a ticket confirmation with price
void displayTicketConfirmation(const Reservation* reservation, char places[][20], int s, int d, float startingTimes[V]) {
    printf("\n\n");
    printf("***************************************\n");
    printf("********* TICKET CONFIRMATION *********\n");
    printf("***************************************\n");
    printf("Booking under Name: %s\n", reservation->name);
    printf("Age: %d\n", reservation->age);
    printf("Departure Time: %.2f IST\n", startingTimes[s]);
    printf("Source: %s\n", places[s]);
    printf("Destination: %s\n", places[d]);
    printf("Number of seats reserved: %d\n",reservation->seatCount);
    printf("Reserved Seats Numbers: ");
    for (int i = 0; i < reservation->seatCount; i++) {
        printf("%d ", reservation->seats[i]);
    }
    printf("\n");
    printf("\nTotal Price: Rs.%d\n",((reservation->seatCount)*(rprice[chooseRoute])));
    printf("***************************************\n");
    printf("***************************************\n");
    printf("\n\n");
}

// Function to print a path
void printPath(int path[], int pathLen, char places[][20], int price,int route) {
    printf("Route %d : ",route);
    for (int i = 0; i < pathLen; i++) {
        printf("%s", places[path[i]]);
        if (i < pathLen - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
    printf("Total Price: Rs.%d per seat\n", price);
    rprice[route]=price;
    printf("\n");
}


void resetCanceledSeats(int busSeats[][row],int canceledSeats[], int numSeatsToCancel) {
    for (int i = 0; i < numSeatsToCancel; i++) {
        int canceledSeat = canceledSeats[i];

        // Search for the original position in the originalBusSeats matrix
        for (int j = 0; j < row; j++) {
            for (int k = 0; k < row; k++) {
                if (originalBusSeats[j][k] == canceledSeat) {
                    // Reset the busSeats matrix to the original seat number
                    busSeats[j][k] = canceledSeat;
                    break;
                }
            }
        }
    }
}

void findAllPaths(int graph[V][V], int src, int dest, bool visited[], int path[], int pathLen, char places[][20], int prices[V][V], int price) {
    visited[src] = true;
    path[pathLen] = src;
    pathLen++;

    if (src == dest) {
        route++;
        printPath(path, pathLen, places, price,route);
    } else {
        for (int i = 0; i < V; i++) {
            if (graph[src][i] && !visited[i]) {
                findAllPaths(graph, i, dest, visited, path, pathLen, places, prices, price + prices[src][i]);
            }
        }
    }

    visited[src] = false;
}

int main() {
    // Initialize all
    Stack seatStack;
    int bookt;
    initializeStack(&seatStack);
    initializerprice(rprice);
    initializeMatrix(busSeats);

    // Initially, push 16 seats onto the stack
    for (int i = 1; i <= MAX_SEATS; i++) {
        push(&seatStack, 1);
    }

    int numSeatsToReserve;
    int reservedSeats[MAX_SEATS];
    int reservedCount = 0;
    Reservation reservation;
    reservation.seatCount = 0;

    char places[V][20] = {"Bangalore", "Mysore", "Mangaluru", "Tamil Nadu", "Kerala", "Shimla"};
    int prices[V][V] = {
        {0, 150, 200, 0, 0, 0},
        {150, 0, 0, 300, 0, 0},
        {200, 0, 0, 0, 250, 0},
        {0, 300, 0, 0, 350, 0},
        {0, 0, 250, 350, 0, 400},
        {0, 0, 0, 0, 400, 0}
    };
    float startingTimes[V] = {5.10, 8.15, 9.30, 12.45, 14.30, 21.15};

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Welcome to A3J Bus Reservation System \n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    // Get name and age
    getPassengerInfo(&reservation);

    int source, destination;
    // Show available places
    printf("Available places:\n");
    for (int i = 0; i < V; i++) {
        printf("%d. %s\n", i, places[i]);
    }

    printf("Enter your source (0-5): ");
    scanf("%d", &source);
    printf("Enter your destination (0-5): ");
    scanf("%d", &destination);
    if (source < 0 || source >= V || destination < 0 || destination >= V) {
        printf("Invalid source or destination.");
    } else if (source == destination) {
        printf("Source and destination are the same.");
    } else {
        bool visited[V] = {false};
        int path[V];
        int pathLen = 0;
        printf("All paths between %s and %s:\n\n", places[source], places[destination]);
        findAllPaths(prices, source, destination, visited, path, pathLen, places, prices, 0);
    }


    printf("Enter the Route which you want to travel in: ",places[source],places[destination]);
    scanf("%d",&chooseRoute);
    rprice[chooseRoute];

    if(rprice[chooseRoute]==0)
    {
        printf("You have entered an invalid route");
        exit(0);
    }

    while (1) {
        printf("\n1. Reserve seats\n");
        printf("2. Cancel seats\n");
        printf("3. Display seat availability\n");
        printf("4. Confirm Reservation\n");
        printf("5. Quit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                 displaySeatAvailability(busSeats);
                printf("Enter the number of seats to reserve: ");
                scanf("%d", &numSeatsToReserve);

                if (numSeatsToReserve > 0 && numSeatsToReserve <= seatStack.top + 1)
                {
                    for (int i = 0; i < numSeatsToReserve; i++)
                    {
                        printf("Enter the seat number you want to book: ");
                        scanf("%d",&bookt);
                        reservation.seats[i]=bookt;
                        reservation.seatCount++;
                        bookSeat(busSeats,bookt,&seatStack);

                    }
                }
                else {
                        printf("Not enough seats available for reservation.\n");
                }
                break;

            

            case 2:
                if (reservation.seatCount > 0) {
                    printf("Enter the number of seats to cancel: ");
                    int numSeatsToCancel;
                    scanf("%d", &numSeatsToCancel);
                    
                    if (numSeatsToCancel > 0 && numSeatsToCancel <= reservation.seatCount) {
                        int canceledSeats[MAX_SEATS];
                        for (int i = 0; i < numSeatsToCancel; i++) {
                            printf("Enter the seat number you want to cancel: ");
                            int cancelSeat;
                            scanf("%d", &cancelSeat);

                            // Check if the entered seat is valid
                            int validSeat = 0;
                            for (int j = 0; j < reservation.seatCount; j++) {
                                if (reservation.seats[j] == cancelSeat) {
                                    validSeat = 1;
                                    break;
                                }
                            }

                            if (validSeat) {
                                push(&seatStack, cancelSeat);
                                canceledSeats[i] = cancelSeat;

                                // Update the reservation data
                                for (int j = 0; j < reservation.seatCount; j++) {
                                    if (reservation.seats[j] == cancelSeat) {
                                        for (int k = j; k < reservation.seatCount - 1; k++) {
                                            reservation.seats[k] = reservation.seats[k + 1];
                                        }
                                        reservation.seatCount--;
                                        break;
                                    }
                                }
                                printf("Seat %d cancelled successfully.\n", cancelSeat);
                            } else {
                                printf("Invalid seat number %d. Please enter a valid seat number.\n", cancelSeat);
                                // Move the cursor back by one to re-enter the same seat number
                                i--;
                            }
                        }

                        // Reset canceled seat numbers in the busSeats matrix
                        resetCanceledSeats(busSeats, canceledSeats, numSeatsToCancel);
                    } else {
                        printf("Invalid number of seats to cancel.\n");
                    }
                } else {
                    printf("No reserved seats to cancel.\n");
                }
                break;


            case 3:
                displaySeatAvailability(busSeats);
                break;

            case 4:
                if (reservation.seatCount > 0) {
                    displayTicketConfirmation(&reservation, places, source, destination, startingTimes);
                    exit(0);
                } else {
                    printf("No seats reserved. Please reserve seats before confirming.\n");
                }
                break;

            case 5:
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
