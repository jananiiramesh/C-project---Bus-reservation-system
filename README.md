# C-project---Bus-reservation-system
README for A3J Bus Reservation System Code

Introduction:

It is simple bus reservation system written in C.
It allows users to reserve seats on a bus, cancel reservations, display seat availability, and confirm their reservation with a ticket.
Key Components:

Stack Implementation:

The stack is initialized and seats are pushed onto it during the program's initialization.
The push and pop functions are used to manage the stack.
Bus Seat Matrix:

The bus seat availability is represented using a 2D array (busSeats).
The original seat arrangement is stored in originalBusSeats.
Reservation Structure:

The Reservation structure holds passenger information (name, age) and the reserved seats.
It is used to keep track of the current reservation.
Graph and Path Finding:

The program finds all paths between source and destination using a graph representation.
The findAllPaths function is responsible for this and uses recursive backtracking.
User Interaction:

Users interact with the program through a menu-driven interface.
They can reserve seats, cancel reservations, view seat availability, and confirm their reservation.
Data Validation:

The program includes checks for invalid inputs, such as an invalid age or seat number.
It ensures that users cannot reserve more seats than available or cancel more seats than reserved.
Ticket Confirmation:

Once seats are reserved, users can confirm their reservation, and a ticket confirmation is displayed.
The confirmation includes passenger details, reserved seats, departure time, source, destination, and total price.
Route and Price:

Users can choose a specific route from the available paths.
The price for each route is calculated using weighted graphs and stored in the rprice array.
