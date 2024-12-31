/*          OM SARASWATI NAMASTUBHYAM VARDE KAMRUPINI VIDHYARAMBHAM 
                    KARISHYAMI SIDDHIRBHAVATU MEIN SADA
*/
#include "./include/api.hpp"
#include "bits/stdc++.h"
using namespace std;
int main(){
    API api("s9tKxLkY", "zdteRn00AM0nSAfyhqcDaMU76mh2lysxr5RThahle7c");
    api.authenticate();
    api.ViewPositions("BTC", "future");
    // api.Get_Order("order_id");
    // api.Cancel_Order("order_id");
    // api.Modify_Order("order_id", 0.0, 0.0);
    return 0;
}