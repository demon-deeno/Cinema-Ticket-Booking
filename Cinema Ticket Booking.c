// WAP to book movie tickets with three price levels and seat allotment.
#include <stdio.h>

#define MAX_SEATS 30
#define SEATS_PER_ROW 5

// Global array to track booked seats (1 = booked,0 = available)
int booked_seats[MAX_SEATS + 1] = {0};

void show_seat_layout(void)
{
    int i, row;
    printf("\n--- Seat Layout ---\n");
    for (row = 1; row <= MAX_SEATS / SEATS_PER_ROW; row++) {
        printf("Row %d: ", row);
        for (i = 1; i <= SEATS_PER_ROW; i++) {
            int seat_no = (row - 1) * SEATS_PER_ROW + i;
            if (booked_seats[seat_no] == 1) {
                printf("X ");
            } else {
                printf("%d ", seat_no);
            }
        }
        printf("\n");
    }
}

int main()
{
    int choice, tickets, price, total, category, start_seat;
    int i, seats_available;

    while (1) {
        printf("\n1. Book Seats\n");
        printf("2. Show Available Seats\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        if (choice == 3) {
            printf("Thank you for visiting!\n");
            break;
        }
        if (choice == 2) {
            show_seat_layout();
            continue;
        }
        if (choice != 1) {
            printf("Invalid choice! Please choose again.\n");
            continue;
        }

        printf("Enter the number of tickets you want to book: ");
        if (scanf("%d", &tickets) != 1) {
            while (getchar() != '\n');
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        if (tickets <= 0 || tickets > MAX_SEATS) {
            printf("Invalid number of tickets! Please enter a value between 1 and %d.\n", MAX_SEATS);
            continue;
        }

        printf("\nSelect Price Category:\n");
        printf("1. Standard - Rs. 150 per ticket\n");
        printf("2. Premium - Rs. 250 per ticket\n");
        printf("3. VIP - Rs. 350 per ticket\n");
        printf("Enter your choice (1-3): ");
        if (scanf("%d", &category) != 1) {
            while (getchar() != '\n');
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        printf("Enter starting seat number: ");
        if (scanf("%d", &start_seat) != 1) {
            while (getchar() != '\n');
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        if (start_seat < 1 || start_seat > MAX_SEATS) {
            printf("Invalid seat number! Available seats are 1 to %d\n", MAX_SEATS);
            continue;
        }
        if (start_seat + tickets - 1 > MAX_SEATS) {
            printf("Not enough seats available! You can book maximum %d seats from seat %d\n", MAX_SEATS - start_seat + 1, start_seat);
            continue;
        }

        if (category == 1)
            price = 150;
        else if (category == 2)
            price = 250;
        else if (category == 3)
            price = 350;
        else {
            printf("Invalid choice!\n");
            continue;
        }

        seats_available = 1;
        for (i = start_seat; i < start_seat + tickets; i++) {
            if (booked_seats[i] == 1) {
                seats_available = 0;
                break;
            }
        }
        if (seats_available == 0) {
            printf("\n--- BOOKING FAILED ---\n");
            printf("Sorry! Seats are already booked in the range %d to %d\n", start_seat, start_seat + tickets - 1);
            printf("Please select different seats.\n");
            continue;
        }

        for (i = start_seat; i < start_seat + tickets; i++) {
            booked_seats[i] = 1;
        }
        total = tickets * price;

        printf("\n--- Ticket Booking Successful ---\n");
        printf("Number of tickets: %d\n", tickets);
        printf("Ticket Class: ");
        if (category == 1)
            printf("Standard\n");
        else if (category == 2)
            printf("Premium\n");
        else if (category == 3)
            printf("VIP\n");
        printf("Price per ticket: Rs. %d\n", price);
        printf("\n--- Seat Details ---\n");
        for (i = start_seat; i < start_seat + tickets; i++) {
            int row_num = (i - 1) / SEATS_PER_ROW + 1;
            int seat_in_row = (i - 1) % SEATS_PER_ROW + 1;
            printf("Seat %d -> Row %d, Position %d\n", i, row_num, seat_in_row);
        }
        printf("\nTotal amount to be paid: Rs. %d\n", total);
        printf("Booking Confirmed!\n");
        show_seat_layout();
    }

    return 0;
}
