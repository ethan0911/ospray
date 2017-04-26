#define CATCH_CONFIG_MAIN
#include "testing/catch.hpp"

#include "box.h"
#include "vec.h"
#include "OSPAny.h"

using ospcommon::OSPAny;

// Helper types ///////////////////////////////////////////////////////////////

struct OSPObject_T {};
using OSPObject = OSPObject_T*;

// Helper functions ///////////////////////////////////////////////////////////

template <typename T>
inline void verify_value(const OSPAny &v, const T &correctValue)
{
  REQUIRE(v.valid());
  REQUIRE(v.is<T>());
  REQUIRE(v.get<T>() == correctValue);
}

template <typename T>
inline void test_interface(T testValue, T testValue2)
{
  OSPAny v;
  REQUIRE(!v.valid());

  SECTION("Can make valid by construction")
  {
    OSPAny v2(testValue);
    verify_value<T>(v2, testValue);
  }

  SECTION("Can make valid by calling operator=()")
  {
    v = testValue;
    verify_value<T>(v, testValue);
  }

  SECTION("Can make valid by copy construction")
  {
    v = testValue;
    OSPAny v2(v);
    verify_value<T>(v2, testValue);
  }

  SECTION("Two objects with same value are equal if constructed the same")
  {
    v = testValue;
    OSPAny v2 = testValue;
    REQUIRE(v == v2);
  }

  SECTION("Two objects with same value are equal if assigned from another")
  {
    v = testValue;
    OSPAny v2 = testValue2;
    v = v2;
    REQUIRE(v == v2);
  }

  SECTION("Two objects with different values are not equal")
  {
    v = testValue;
    OSPAny v2 = testValue2;
    REQUIRE(v != v2);
  }
}

// Tests //////////////////////////////////////////////////////////////////////

TEST_CASE("OSPAny 'int' type behavior", "[types]")
{
  test_interface<int>(5, 7);
}

TEST_CASE("OSPAny 'float' type behavior", "[types]")
{
  test_interface<float>(1.f, 2.f);
}

TEST_CASE("OSPAny 'bool' type behavior", "[types]")
{
  test_interface<bool>(true, false);
}

TEST_CASE("OSPAny 'vec3f' type behavior", "[types]")
{
  test_interface<ospcommon::vec3f>({1.f, 1.f, 1.f}, {2.f, 3.f, 4.f});
}

TEST_CASE("OSPAny 'vec2f' type behavior", "[types]")
{
  test_interface<ospcommon::vec2f>({1.f, 1.f}, {3.f, 4.f});
}

TEST_CASE("OSPAny 'vec2i' type behavior", "[types]")
{
  test_interface<ospcommon::vec2i>({1, 1}, {3, 4});
}

TEST_CASE("OSPAny 'box3f' type behavior", "[types]")
{
  test_interface<ospcommon::box3f>({{1.f, 1.f, 1.f}, {2.f, 2.f, 2.f}},
                                   {{3.f, 4.f, 5.f}, {6.f, 7.f, 8.f}});
}

TEST_CASE("OSPAny 'string' type behavior", "[types]")
{
  test_interface<std::string>("Hello", "World");
}

TEST_CASE("OSPAny 'OSPObject' type behavior", "[types]")
{
  // NOTE(jda) - we just need some phony pointer addresses to test OSPAny,
  //             no need to hand it "real" OSPRay objects...
  void *val1 = nullptr;
  void *val2 = nullptr;
  val1 = &val1;
  val2 = &val2;

  test_interface<OSPObject>(static_cast<OSPObject>(val1),
                            static_cast<OSPObject>(val2));
}