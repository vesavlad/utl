#include "../catch.hpp"

#include "utl/parser/buf_reader.h"
#include "utl/parser/csv_range.h"
#include "utl/parser/line_range.h"
#include "utl/pipes/avg.h"
#include "utl/pipes/remove_if.h"
#include "utl/pipes/transform.h"

using namespace utl;

constexpr auto const input = R"(date,time,open,high,low,close,volume
01/02/1998,09:30,39.04,39.04,39.04,39.04,179332
01/02/1998,09:31,39,39.04,39,39,28907
01/02/1998,09:32,39.02,39.04,39,39.04,35599
01/02/1998,09:33,39.02,39.04,39,39,44967
01/02/1998,09:34,39,39.04,39,39,12850
01/02/1998,09:35,39.02,39.04,39,39,62365
01/02/1998,09:36,39.02,39.02,39,39.02,6959
01/02/1998,09:37,39,39,39,39,7494
01/02/1998,09:38,39,39.02,39,39.02,29713)";

struct quote {
  csv_col<float, name("open")> open;
  csv_col<float, name("high")> high;
  csv_col<float, name("low")> low;
  csv_col<float, name("close")> close;
  csv_col<int, name("volume")> volume;
  csv_col<cstr, name("date")> date;
  csv_col<cstr, name("time")> time;
};

TEST_CASE("csv") {
  auto const avg_volume =
      line_range<buf_reader>{buf_reader{input}}  //
      | csv<quote>()  //
      | remove_if([](auto&& row) { return row.open < 39.01; })  //
      | transform([](auto&& row) { return row.volume; })  //
      | avg();
  CHECK(avg_volume == 65844);
}