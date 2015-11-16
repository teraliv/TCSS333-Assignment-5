/*
 *  main.c
 *  TCSS 333 - Autumn 2015
 *
 *  Assignment 5 - Using an Array of Nested Struct(s)
 *  Alex Terikov (teraliv@uw.edu)
 *  11/12/15
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct order_tag {
    int quantity;
    char product[10];
    double price;
};

typedef struct order_tag Order;

struct customer_tag {
    char name[10];
    int orders;
    double total;
    Order ord[10];
};

typedef struct customer_tag Customer;



int addAllCustomers(FILE *file, Customer *p);
int findCustomer(Customer *q, char *name, int index);
void writeChronologicalListing(Customer customers[], int size);
void writeFinancialListing(Customer customers[], int size);
void bubbleSortTotalPurchases(Customer customers[], int size);
void bubbleSortSinglePurchase(Customer customers[], int size);
void printCustomers(Customer customers[], int size);
double getPrice(char *price);



int main() {
    
    FILE *inFile = fopen("hw5input.txt", "r");
    
    if (!inFile) {
        printf("File is not found!\n");
    }
    
    Customer customers[10];
    
    int size;
    
    size = addAllCustomers(inFile, customers);

    //printCustomers(customers, size);
    writeChronologicalListing(customers, size);
    bubbleSortTotalPurchases(customers, size);
    bubbleSortSinglePurchase(customers, size);
    writeFinancialListing(customers, size);
    
    fclose(inFile);
    
    return 0;
}


/*
 * This function read customers data from the given file
 * and creates a struct array of this data.
 *
 * @param *file - text file to read data.
 * @param *p - pointer to a Customer structure.
 * @return - returns total number of customers added to a struct.
 */
int addAllCustomers(FILE *file, Customer *p) {
    Customer *q = p;
    Customer *z = p;

    int size = 0;
    int orderNum = 0;
    int totalCustomers = 0;
    int qty;
    int loc;
    
    char *name = malloc(sizeof(char) * 10);
    char *product = malloc(sizeof(char) * 10);
    char *price = malloc(sizeof(char) * 10);
    
    Customer currCustomer;
    Order currOrder;
    
    // read line by line from txt file
    while (fscanf(file, "%s %d %s %s", name, &qty, product, price) != EOF) {
        
        // arrays are not assignable so using strcpy function
        strcpy(currCustomer.name, name);
        strcpy(currOrder.product, product);
        
        // index location for the current customer
        loc = findCustomer(q, name, size);
        
        currOrder.quantity = qty;
        
        // get double price value from string
        currOrder.price = getPrice(price);
        
        // customer already exists, so add just new ortder data to order struct
        if (loc >= 0) {
            ((z + loc) -> orders) += 1;
            orderNum = (z + loc) -> orders;
            (z + loc) -> ord[orderNum] = currOrder;
            (z + loc) -> total += qty * getPrice(price);
        }
        
        // new customer
        if (loc == -1) {
            currCustomer.ord[0] = currOrder;
            currCustomer.orders = 0;
            currCustomer.total = qty * getPrice(price);
            *p = currCustomer;
            p++;
            size++;
            totalCustomers++;
        }
        
    }

    return totalCustomers;
}


/*
 * This function check if the given customer exists
 * in the customer struct.
 *
 * @param *p - pointer to a Customer structure.
 * @param *cname - customer's name.
 * @param size - current total number of customers.
 * @return - returns -1 if customer is not found, otherwise returns 
 *           index location of the given customer.
 */
int findCustomer(Customer *p, char *cname, int size) {
    
    int i;
    int location = 0;
    int count = 0;
    int found = 0;
    int result;

    for (i = 0; i <= size; i++) {
        if (strcmp((*p).name, cname) == 0) {
            found = 1;
            location = count;
        }
        p++;
        count++;
    }
    
    if (found == 1)
        result = location;
    else
        result = -1;
    
    return result;
}


/*
 * This function write the all customers data to the
 * txt file in chronological order.
 *
 * @param customers[] - struct array of all customers and their orders.
 * @param size - the total number of customers.
 */
void writeChronologicalListing(Customer customers[], int size) {
    
    FILE *outFile = fopen("hw5time.txt", "w");
    
    int i, j, qty;
    double price;
    
    for (i = 0; i < size; i++) {
        fprintf(outFile, "Customer: %s\n", customers[i].name);
        fprintf(outFile, "%15s\n", "Orders: ");
        
        for (j = 0; j <= customers[i].orders; j++) {
            qty = customers[i].ord[j].quantity;
            price = customers[i].ord[j].price;
            
            fprintf(outFile, "%15s", "");
            fprintf(outFile, "%-15s", customers[i].ord[j].product);
            fprintf(outFile, "%-7d", qty);
            fprintf(outFile, "%7.2f", price);
            fprintf(outFile, "%10.2f\n", qty * price);
            
            
        }
        fprintf(outFile, "%45s", "Total: ");
        fprintf(outFile, "%9.2f\n", customers[i].total);
        fprintf(outFile, "\n");
    }
    
    fclose(outFile);
}


/*
 * This function write the all customers data to the
 * txt file in financial order.
 *
 * @param customers[] - struct array of all customers and their orders.
 * @param size - the total number of customers.
 */
void writeFinancialListing(Customer customers[], int size) {
    
    FILE *outFile = fopen("hw5money.txt", "w");
    
    int i, j, qty;
    double price;
    
    for (i = 0; i < size; i++) {
        fprintf(outFile, "%s, ", customers[i].name);
        fprintf(outFile, "Total order = $%.2f\n", customers[i].total);
        
        for (j = 0; j <= customers[i].orders; j++) {
            qty = customers[i].ord[j].quantity;
            price = customers[i].ord[j].price;
            
            fprintf(outFile, "%s ", customers[i].ord[j].product);
            fprintf(outFile, "%d ", qty);
            fprintf(outFile, "$%.2f, ", price);
            fprintf(outFile, "Item value = $%.2f\n", qty * price);
        }
        fprintf(outFile, "\n");
    }
    
    fclose(outFile);

}


/*
 * This function do bubble sort based on the total value
 * of all purchases.
 *
 * @param customers[] - struct array of all customers and their orders.
 * @param size - the total number of customers.
 */
void bubbleSortTotalPurchases(Customer customers[], int size) {
    
    int i, j;
    int notSorted = 1;
    
    Customer temp;
    
    for (i = 0; i < size - 1 && notSorted; i++) {
        for (j = 0, notSorted = 0; j < size - 1; j++) {
            
            if (customers[j].total < customers[j+1].total) {
                temp = customers[j];
                customers[j] = customers[j + 1];
                customers[j + 1] = temp;
                notSorted = 1;
            }
            
        }
    }
    
}


/*
 * This function do bubble sort based on the single value purchase.
 *
 * @param customers[] - struct array of all customers and their orders.
 * @param size - the total number of customers.
 */
void bubbleSortSinglePurchase(Customer customers[], int size) {
    
    int i, j, k;
    double ltotal, rtotal;
    
    Order ord;

    for (i = 0; i < size - 1; i++) {
        
        for (j = 0; j < customers[i].orders; j++) {
            
            for (k = 0; k < customers[i].orders; k++) {
                
                ltotal = customers[i].ord[k].quantity * customers[i].ord[k].price;
                rtotal = customers[i].ord[k + 1].quantity * customers[i].ord[k + 1].price;
                
                if (ltotal < rtotal) {
                    ord = customers[i].ord[k];
                    customers[i].ord[k] = customers[i].ord[k + 1];
                    customers[i].ord[k + 1] = ord;
                }
            }
        }
    }

}


/*
 * This function prints all customers data to the screen.
 *
 * @param customers[] - struct array of all customers and their orders.
 * @param size - the total number of customers.
 */
void printCustomers(Customer customers[], int size) {

    int i, j, qty;
    double price;
    
    for (i = 0; i < size; i++) {
        printf("Customer: %s\n", customers[i].name);
        printf("%15s\n", "Orders: ");
        
        for (j = 0; j <= customers[i].orders; j++) {
            qty = customers[i].ord[j].quantity;
            price = customers[i].ord[j].price;
            
            printf("%15s", "");
            printf("%-15s", customers[i].ord[j].product);
            printf("%-7d", qty);
            printf("%7.2f", price);
            printf("%10.2f\n", qty * price);
            
            
        }
        printf("%45s", "Total: ");
        printf("%9.2f\n", customers[i].total);
        printf("\n");
    }
}

/*
 * This function get double value of the price 
 * from the given string.
 *
 * @return - return price.
 */
double getPrice(char *str) {
    double price;
    char *p = str;
    p++;
    price = atof(p);
    
    return price;
}