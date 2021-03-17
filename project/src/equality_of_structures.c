#include "equality_of_structures.h"

int structs_equal(Record expected_client_data, Record got_client_data) {
  double eps = 1e-2;
  if (expected_client_data.number == got_client_data.number &&
    strcmp(expected_client_data.name, got_client_data.name) == 0 &&
    strcmp(expected_client_data.surname, got_client_data.surname) == 0 &&
    strcmp(expected_client_data.address, got_client_data.address) == 0 &&
    strcmp(expected_client_data.phone, got_client_data.phone) == 0 &&
    fabs(expected_client_data.indebtedness - got_client_data.indebtedness) <= eps &&
    fabs(expected_client_data.credit_limit - got_client_data.credit_limit) <= eps &&
    fabs(expected_client_data.cash_payments - got_client_data.cash_payments) <= eps) {
      return 0;
  }
  return -1;
}
