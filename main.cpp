/*          OM SARASWATI NAMASTUBHYAM VARDE KAMRUPINI VIDHYARAMBHAM 
                    KARISHYAMI SIDDHIRBHAVATU MEIN SADA
*/
#include "./include/api.hpp"
#include "bits/stdc++.h"
using namespace std;
int main(){
    API api("s9tKxLkY", "zdteRn00AM0nSAfyhqcDaMU76mh2lysxr5RThahle7c");
    api.authenticate();
    int choice;
    cout << "1. View Positions\n2. Get Order\n3. Modify Order\n4. Cancel Order\n5. Place Order\n ^C to exit\n Enter your choice: ";
    cin >> choice;

    string order_id;
    double price, amount, quantity;
    switch (choice){
        case 1:
            api.ViewPositions("BTC", "future");
            break;
        case 2:
            cout << "Enter order id: ";
            cin >> order_id;
            api.Get_Order(order_id);
            break;
        case 3:
            cout << "Enter order id: ";
            cin >> order_id;
            cout << "Enter new price: ";
            cin >> price;
            cout << "Enter new amount: ";
            cin >> amount;
            api.Modify_Order(order_id, price, amount);
            break;
        case 4:
            cout << "Enter order id: ";
            cin >> order_id;
            api.Cancel_Order(order_id);
            break;
        //  void Place_Order(string name, double price, double quantity)
        case 5:
            string name;
            cout << "Enter instrument name: ";
            cin >> name;
            cout << "Enter price: ";
            cin >> price;
            cout << "Enter quantity: ";
            cin >> quantity;
            api.Place_Order(name, price, quantity);
            break;
    }
    return 0;
}