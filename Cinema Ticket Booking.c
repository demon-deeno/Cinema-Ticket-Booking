// WAP to book movie tickets with three price levels and seat allotment.
#include <stdio.h>

#define MAX_SEATS 56
#define SEATS_PER_ROW 6

// Global array to track booked seats (1 = booked,0 = available)
int booked_seats[MAX_SEATS + 1] = {0};

// Prints the current seat layout, showing 'X' for booked seats and the seat number for available ones.
void show_seat_layout(void)
{
    int i, row, rows;
    printf("\n--- Seat Layout ---\n");
    rows = (MAX_SEATS + SEATS_PER_ROW - 1) / SEATS_PER_ROW;
    for (row = 1; row <= rows; row++) {
        printf("Row %d: ", row);
        for (i = 1; i <= SEATS_PER_ROW; i++) {
            int seat_no = (row - 1) * SEATS_PER_ROW + i;
            if (seat_no > MAX_SEATS)
                break;
            if (booked_seats[seat_no] == 1) {
                printf("X ");
            } else {
                printf("%d ", seat_no);
            }
        }
        printf("\n");
    }
    /* Show category legend based on row mapping */
    printf("\nCategory layout: Rows 1-6 = Standard, Rows 7-8 = Premium, Rows 9+ = VIP\n");
}

/* Return category for a given seat number: 1=Standard,2=Premium,3=VIP */
int seat_category(int seat_no)
{
    if (seat_no < 1 || seat_no > MAX_SEATS)
        return 0;
    int row = (seat_no - 1) / SEATS_PER_ROW + 1;
    if (row <= 6)
        return 1; /* Standard */
    else if (row <= 8)
        return 2; /* Premium */
    else
        return 3; /* VIP */
}

// Runs the main menu loop: books tickets, shows seat availability, or exits.
int main()
{
    int choice, tickets, total, category, start_seat;
    int i, seats_available;
    int ticketCategories[MAX_SEATS] = {0};
    int ticketPrices[MAX_SEATS] = {0};

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

        printf("\nSelect ticket type:\n");
        printf("1. Standard - Rs. 150 per ticket\n");
        printf("2. Premium - Rs. 250 per ticket\n");
        printf("3. VIP - Rs. 350 per ticket\n");
        printf("4. Auto-assign category by seat (predefined layout)\n");
        printf("5. Specify counts per category (e.g. 3 Standard,2 VIP,1 Premium)\n");
        printf("Enter your choice (1-5): ");
        if (scanf("%d", &category) != 1) {
            while (getchar() != '\n');
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        if (category == 5) {
            int needStd = 0, needPrem = 0, needVIP = 0;
            printf("Enter number of Standard tickets: ");
            if (scanf("%d", &needStd) != 1) { while (getchar() != '\n'); printf("Invalid input!\n"); continue; }
            printf("Enter number of Premium tickets: ");
            if (scanf("%d", &needPrem) != 1) { while (getchar() != '\n'); printf("Invalid input!\n"); continue; }
            printf("Enter number of VIP tickets: ");
            if (scanf("%d", &needVIP) != 1) { while (getchar() != '\n'); printf("Invalid input!\n"); continue; }
            if (needStd < 0 || needPrem < 0 || needVIP < 0) { printf("Counts cannot be negative.\n"); continue; }
            if (needStd + needPrem + needVIP != tickets) { printf("The sum of counts must equal total tickets (%d).\n", tickets); continue; }

            /* Find available seats for each category anywhere in the house */
            int assignedSeats[MAX_SEATS] = {0};
            int temp_taken[MAX_SEATS + 1] = {0};
            int idx = 0, seat_no;

            /* Allocate Standard */
            for (seat_no = 1; seat_no <= MAX_SEATS && needStd > 0; seat_no++) {
                if (booked_seats[seat_no] || temp_taken[seat_no]) continue;
                if (seat_category(seat_no) == 1) {
                    assignedSeats[idx] = seat_no;
                    ticketCategories[idx] = 1;
                    ticketPrices[idx] = 150;
                    temp_taken[seat_no] = 1;
                    idx++; needStd--;
                }
            }
            /* Allocate Premium */
            for (seat_no = 1; seat_no <= MAX_SEATS && needPrem > 0; seat_no++) {
                if (booked_seats[seat_no] || temp_taken[seat_no]) continue;
                if (seat_category(seat_no) == 2) {
                    assignedSeats[idx] = seat_no;
                    ticketCategories[idx] = 2;
                    ticketPrices[idx] = 250;
                    temp_taken[seat_no] = 1;
                    idx++; needPrem--;
                }
            }
            /* Allocate VIP */
            for (seat_no = 1; seat_no <= MAX_SEATS && needVIP > 0; seat_no++) {
                if (booked_seats[seat_no] || temp_taken[seat_no]) continue;
                if (seat_category(seat_no) == 3) {
                    assignedSeats[idx] = seat_no;
                    ticketCategories[idx] = 3;
                    ticketPrices[idx] = 350;
                    temp_taken[seat_no] = 1;
                    idx++; needVIP--;
                }
            }

            if (idx != tickets) {
                printf("Not enough available seats in the requested categories.\n");
                continue;
            }

            /* Mark seats booked and prepare for receipt */
            for (i = 0; i < tickets; i++) {
                int s = assignedSeats[i];
                booked_seats[s] = 1;
            }
            /* Compute total and print receipt using assignedSeats */
            total = 0;
            for (i = 0; i < tickets; i++) total += ticketPrices[i];

            printf("\n--- Ticket Booking Successful ---\n");
            printf("Number of tickets: %d\n", tickets);
            printf("\n--- Seat Details ---\n");
            for (i = 0; i < tickets; i++) {
                int seat = assignedSeats[i];
                int row_num = (seat - 1) / SEATS_PER_ROW + 1;
                int seat_in_row = (seat - 1) % SEATS_PER_ROW + 1;
                printf("Seat %d -> Row %d, Position %d, Category: %s, Price: Rs. %d\n",
                       seat, row_num, seat_in_row,
                       ticketCategories[i] == 1 ? "Standard" : ticketCategories[i] == 2 ? "Premium" : "VIP",
                       ticketPrices[i]);
            }
            printf("\nTotal amount to be paid: Rs. %d\n", total);
            printf("Booking Confirmed!\n");
            show_seat_layout();
            continue; /* Done this booking iteration */
        } else {
            /* For category 1-4 we need a starting seat */
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

            if (category >= 1 && category <= 3) {
                int price = (category == 1 ? 150 : category == 2 ? 250 : 350);
                for (i = 0; i < tickets; i++) {
                    ticketCategories[i] = category;
                    ticketPrices[i] = price;
                }
            } else if (category == 4) {
                /* Auto-assign category based on predefined seat layout */
                for (i = 0; i < tickets; i++) {
                    int seat_no = start_seat + i;
                    int c = seat_category(seat_no);
                    ticketCategories[i] = c;
                    ticketPrices[i] = (c == 1 ? 150 : c == 2 ? 250 : 350);
                }
            } else {
                printf("Invalid choice!\n");
                continue;
            }

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
        total = 0;
        for (i = 0; i < tickets; i++) {
            total += ticketPrices[i];
        }

        printf("\n--- Ticket Booking Successful ---\n");
        printf("Number of tickets: %d\n", tickets);
        if (category == 4) {
            printf("Ticket Classes: Standard, Premium, VIP\n");
        } else {
            printf("Ticket Class: %s\n", ticketCategories[0] == 1 ? "Standard" : ticketCategories[0] == 2 ? "Premium" : "VIP");
        }
        printf("\n--- Seat Details ---\n");
        for (i = 0; i < tickets; i++) {
            int seat_num = start_seat + i;
            int row_num = (seat_num - 1) / SEATS_PER_ROW + 1;
            int seat_in_row = (seat_num - 1) % SEATS_PER_ROW + 1;
            printf("Seat %d -> Row %d, Position %d, Category: %s, Price: Rs. %d\n",
                   seat_num, row_num, seat_in_row,
                   ticketCategories[i] == 1 ? "Standard" : ticketCategories[i] == 2 ? "Premium" : "VIP",
                   ticketPrices[i]);
        }
        printf("\nTotal amount to be paid: Rs. %d\n", total);
        printf("Booking Confirmed!\n");
        show_seat_layout();
    }

    return 0;
}


