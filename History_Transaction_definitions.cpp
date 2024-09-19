#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

// Definitions for Transaction class


// Constructor
Transaction::Transaction( std::string ticker_symbol, unsigned int day_date, unsigned int
month_date, unsigned year_date, bool buy_sell_trans, unsigned int number_shares,
double trans_amount ){

    day = day_date;
    month = month_date;
    year = year_date;
    symbol = ticker_symbol;
    shares = number_shares;
    amount = trans_amount;
    trans_id= assigned_trans_id++;
    p_next=nullptr;
    if (buy_sell_trans) {
    trans_type = "Buy";
} else {
    trans_type = "Sell";
}
     double acb=0;
     unsigned int share_balance=0;
    
    
}
// Destructor
Transaction::~Transaction() {
}

// Overloaded < operator.
bool Transaction::operator<(const Transaction& other) {
    if (year < other.year) {
        return true;
    }
     else if (year==other.year && month < other.month){
        return true;
    } else if (month == other.month && day<other.day) {
        return true;
    }
    else if (year == other.year && month==other.month && day==other.day){
      return trans_id< other.trans_id;
    }

    return false;
}


// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
History::History() {

  p_head=nullptr;
}

// Destructor
History::~History(){
    Transaction *current = p_head;
    Transaction *next;
    while (current != nullptr) {
       
        next = current->get_next();
        delete current;
        current = next;
    }
    p_head = nullptr;
}


// insert(...): Insert transaction into linked list.
void History::insert( Transaction *p_new_trans ){
     if (p_head == nullptr) {
        p_head = p_new_trans;
        p_new_trans->set_next(nullptr);
    } else {
        Transaction* p_current = p_head;

        while (p_current->get_next() != nullptr) {
            p_current = p_current->get_next();
        }
        p_current->set_next(p_new_trans);
        p_new_trans->set_next(nullptr);
    }
}



// read_history(...): Read the transaction history from file.
void History::read_history(){

  ece150::open_file();
  while (ece150::next_trans_entry()){

            std::string ticker_symbol = ece150::get_trans_symbol();
            unsigned int day_date = ece150::get_trans_day();
            unsigned int month_date = ece150::get_trans_month();
            unsigned int year_date = ece150::get_trans_year();
            unsigned int number_shares = ece150::get_trans_shares();
            bool buy_sell_trans = ece150::get_trans_type();
            double trans_amount = ece150::get_trans_amount();

            Transaction *new_trans= new Transaction (ticker_symbol,day_date,
month_date, year_date, buy_sell_trans, number_shares,trans_amount);
            insert(new_trans);
  }

  ece150::close_file ();
}



// print() Print the transaction history.
void History::print(){

    Transaction *current_transaction = p_head;

    std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
    
    while (current_transaction != nullptr) {
        current_transaction->print();
        current_transaction = current_transaction->get_next();
    }

    std::cout << "=========== END TRANSACTION HISTORY =============" << std::endl;
}



// sort_by_date(): Sort the linked list by trade date.
void History::sort_by_date(){
 
    // Check if the list is empty or has only one element (already sorted)
    if (p_head == nullptr || p_head->get_next() == nullptr) {
        return;
    }

    // Create a new linked list to store the sorted transactions
    Transaction* sorted_list = nullptr;

    // Traverse the original list
    while (p_head != nullptr) {
        // Remove the first transaction from the "unsorted" list
        Transaction* current = p_head;
        p_head = p_head->get_next();
        current->set_next(nullptr);

        // Find the correct position to insert the transaction into the "sorted" list
        if (sorted_list == nullptr || *current < *sorted_list) {
            // Insert at the beginning
            current->set_next(sorted_list);
            sorted_list = current;
        } else {
            // Traverse the "sorted" list to find the correct position
            Transaction* temp = sorted_list;
            while (temp->get_next() != nullptr && *temp->get_next() < *current) {
                temp = temp->get_next();
            }

            // Insert in the middle or at the end
            current->set_next(temp->get_next());
            temp->set_next(current);
        }
    }

    // Update the head of the history to point to the sorted list
    p_head = sorted_list;
}



// update_acb_cgl(): Updates the ACB and CGL values.
void History::update_acb_cgl() {
    Transaction *current_transaction = p_head;
    Transaction *previous_transaction = nullptr;

    while (current_transaction != nullptr) {
        if (current_transaction->get_trans_type()) {
            // Buy transaction
            double acb = (previous_transaction != nullptr) ? (previous_transaction->get_acb() + (current_transaction->get_shares() * current_transaction->get_amount())) : current_transaction->get_amount();

            // Update ACB for Buy transaction
            current_transaction->set_acb(acb);

            // Update Share Balance
            current_transaction->set_share_balance(current_transaction->get_shares() + ((previous_transaction != nullptr) ? previous_transaction->get_share_balance() : 0));

            // Update ACB/Share
            current_transaction->set_acb_per_share(acb / current_transaction->get_share_balance());
        } else {
            // Sell transaction
            if (previous_transaction != nullptr) {
                double acb = previous_transaction->get_acb() - (current_transaction->get_shares() * previous_transaction->get_acb_per_share());

                // Update ACB for Sell transaction
                current_transaction->set_acb(acb);

                // Update Share Balance
                unsigned int share_balance = previous_transaction->get_share_balance() - current_transaction->get_shares();
                current_transaction->set_share_balance(share_balance);

                // Update ACB/Share
                double acb_per_share = acb / share_balance;
                current_transaction->set_acb_per_share(acb_per_share);

                // Update CGL
                double cgl = current_transaction->get_amount() - (current_transaction->get_shares() * previous_transaction->get_acb_per_share());
                current_transaction->set_cgl(cgl);
            }
        }

        // Move to the next transaction
        previous_transaction = current_transaction;
        current_transaction = current_transaction->get_next();
    }
}






// compute_cgl(): Compute the ACB, and CGL.
double History::compute_cgl(unsigned int year) {
    double total_cgl = 0.0;
    Transaction *current_transaction = p_head;

    while (current_transaction != nullptr) {
        // Check if the transaction is in the specified year
        if (current_transaction->get_year() == year) {
            if (!current_transaction->get_trans_type()) {
                // Find the previous transaction
                Transaction *previous_transaction = p_head;
                Transaction *temp = p_head;

                while (temp != current_transaction) {
                    previous_transaction = temp;
                    temp = temp->get_next();
                }

                // Calculate CGL for Sell transaction
                double cgl = current_transaction->get_amount() -
                             (current_transaction->get_shares() * previous_transaction->get_acb_per_share());

                // Update CGL for the Sell transaction
                current_transaction->set_cgl(cgl);

                total_cgl += cgl;
            }
        }

        // Move to the next transaction
        current_transaction = current_transaction->get_next();
    }

    return total_cgl;
}


// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
