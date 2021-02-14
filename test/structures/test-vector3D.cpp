#include <catch2/catch_all.hpp>
#include "../../src/structures/vector3D.h"

//TEST_CASE(name of test case, tags for selecting what test cases to run)
TEST_CASE("vector3D tests", "[struct][vector3D]") {
    vector3D vec_main(1, 1, 1);
    vector3D vec_oth(2, 3, 4);
    SECTION("Basic operations") {
        REQUIRE(vec_main[0] == Catch::Approx(1.0).margin(1e-12));
        REQUIRE(vec_main[1] == Catch::Approx(1.0).margin(1e-12));
        REQUIRE(vec_main[2] == Catch::Approx(1.0).margin(1e-12));
        REQUIRE_FALSE((vec_main == vec_oth));
        
        vec_main[0] = 5;
        REQUIRE(vec_main[0] == Catch::Approx(5.0).margin(1e-12));
    }
    //REQUIRE - stops running if failed
    //CHECK - continue running even if failed
    SECTION("Summary operations") {
        vector3D sum_vec = vector3D(3.0, 4.0, 5.0);
        REQUIRE((vec_main + vec_oth == sum_vec));
        
        vec_main += vec_oth;
        CHECK((vec_main == sum_vec));
    }
    SECTION("Difference operations") {
        vector3D dif_vec = vector3D(1.0, 2.0, 3.0);
        REQUIRE((vec_oth - vec_main == dif_vec));
        
        vec_oth -= vec_main;
        CHECK((vec_oth == dif_vec));
    }
    
    SECTION("Product operations") {
        vector3D prod_res = vector3D(2, 2, 2);
        REQUIRE((2 * vec_main == prod_res));
        vec_main *= 2;
        CHECK((vec_main == prod_res)); //vec_main == {2, 2, 2}
        
        REQUIRE(vec_main * vec_oth == Catch::Approx(18.0).margin(1e-12));
        CHECK(((vec_main ^ vec_oth) == vector3D(2, -4, 2)));
    }
    
    SECTION("Numeric Properties") {
        REQUIRE(vec_oth.getLength() == Catch::Approx(std::sqrt(29.0)).margin(1e-12));
        CHECK_FALSE(vec_main.getLength() == vec_oth.getLength());
        CHECK(vec_main.getAngle(vec_oth) == Catch::Approx(std::acos(3.0 * std::sqrt(87.0) / 29.0)).margin(1e-12));
    }
}
