// __BEGIN_LICENSE__
// Copyright (C) 2006-2009 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


// TestGeoReference.h
#include <cxxtest/TestSuite.h>

#include <vw/Cartography/ToastTransform.h>

using namespace vw;
using namespace vw::cartography;

static const int32 toast_resolution = 4*255+1;
static const int32 lonlat_resolution = 4*256;

class TestToastTransform : public CxxTest::TestSuite
{
  static ToastTransform get_basic_transform() {
    GeoReference georef;
    georef.set_pixel_interpretation(GeoReference::PixelAsPoint);
    Matrix3x3 M;
    M(0,0) = 360.0 / lonlat_resolution;
    M(0,2) = -180;
    M(1,1) = -M(0,0);
    M(1,2) = 90;
    M(2,2) = 1;
    georef.set_transform(M);

    return ToastTransform(georef, toast_resolution);
  }
public:

  void test_basic_forward()
  {
    ToastTransform txform = get_basic_transform();
    Vector2 point;

    // North pole
    point = txform.forward(Vector2(0,0));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1)/2, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/2, 1e-5 );

    // North pole
    point = txform.forward(Vector2(lonlat_resolution/4,0));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1)/2, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/2, 1e-5 );

    // North pole
    point = txform.forward(Vector2(lonlat_resolution/2,0));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1)/2, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/2, 1e-5 );

    // North pole
    point = txform.forward(Vector2(3*lonlat_resolution/4,0));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1)/2, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/2, 1e-5 );

    // North pole
    point = txform.forward(Vector2(lonlat_resolution,0));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1)/2, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/2, 1e-5 );

    // (-180,45)
    point = txform.forward(Vector2(0,lonlat_resolution/8));
    TS_ASSERT_DELTA( point.x(), 3*(toast_resolution-1)/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/2, 1e-5 );

    // (-90,45)
    point = txform.forward(Vector2(lonlat_resolution/4,lonlat_resolution/8));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1)/2, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/4, 1e-5 );

    // (0,45)
    point = txform.forward(Vector2(lonlat_resolution/2,lonlat_resolution/8));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1)/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/2, 1e-5 );

    // (90,45)
    point = txform.forward(Vector2(3*lonlat_resolution/4,lonlat_resolution/8));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1)/2, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*(toast_resolution-1)/4, 1e-5 );

    // (180,45)
    point = txform.forward(Vector2(0,lonlat_resolution/8));
    TS_ASSERT_DELTA( point.x(), 3*(toast_resolution-1)/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/2, 1e-5 );

    // (-180,0)
    point = txform.forward(Vector2(0,lonlat_resolution/4));
    TS_ASSERT_DELTA( point.x(), toast_resolution-1, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/2, 1e-5 );

    // (-135,0)
    point = txform.forward(Vector2(lonlat_resolution/8,lonlat_resolution/4));
    TS_ASSERT_DELTA( point.x(), 3*(toast_resolution-1)/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/4, 1e-5 );

    // (-90,0)
    point = txform.forward(Vector2(lonlat_resolution/4,lonlat_resolution/4));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1)/2, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 0, 1e-5 );

    // (-45,0)
    point = txform.forward(Vector2(3*lonlat_resolution/8,lonlat_resolution/4));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1)/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/4, 1e-5 );

    // (0,0)
    point = txform.forward(Vector2(lonlat_resolution/2,lonlat_resolution/4));
    TS_ASSERT_DELTA( point.x(), 0, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/2, 1e-5 );

    // (45,0)
    point = txform.forward(Vector2(5*lonlat_resolution/8,lonlat_resolution/4));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1)/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*(toast_resolution-1)/4, 1e-5 );

    // (90,0)
    point = txform.forward(Vector2(3*lonlat_resolution/4,lonlat_resolution/4));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1)/2, 1e-5 );
    TS_ASSERT_DELTA( point.y(), toast_resolution-1, 1e-5 );

    // (135,0)
    point = txform.forward(Vector2(7*lonlat_resolution/8,lonlat_resolution/4));
    TS_ASSERT_DELTA( point.x(), 3*(toast_resolution-1)/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*(toast_resolution-1)/4, 1e-5 );

    // (180,0)
    point = txform.forward(Vector2(0,lonlat_resolution/4));
    TS_ASSERT_DELTA( point.x(), toast_resolution-1, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1)/2, 1e-5 );

    // (-180,-45)
    point = txform.forward(Vector2(0,3*lonlat_resolution/8));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1), 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*(toast_resolution-1)/4, 1e-5 );

    // (-90,-45)
    point = txform.forward(Vector2(lonlat_resolution/4,3*lonlat_resolution/8));
    TS_ASSERT_DELTA( point.x(), 3*(toast_resolution-1)/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 0, 1e-5 );

    // (0,-45)
    point = txform.forward(Vector2(lonlat_resolution/2,3*lonlat_resolution/8));
    TS_ASSERT_DELTA( point.x(), 0, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*(toast_resolution-1)/4, 1e-5 );

    // (90,-45)
    point = txform.forward(Vector2(3*lonlat_resolution/4,3*lonlat_resolution/8));
    TS_ASSERT_DELTA( point.x(), 3*(toast_resolution-1)/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), toast_resolution-1, 1e-5 );

    // (180,-45)
    point = txform.forward(Vector2(0,3*lonlat_resolution/8));
    TS_ASSERT_DELTA( point.x(), toast_resolution-1, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*(toast_resolution-1)/4, 1e-5 );

    // South pole
    point = txform.forward(Vector2(0,lonlat_resolution/2));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1), 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1), 1e-5 );

    // South pole
    point = txform.forward(Vector2(lonlat_resolution/4,lonlat_resolution/2));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1), 1e-5 );
    TS_ASSERT_DELTA( point.y(), 0, 1e-5 );

    // South pole
    point = txform.forward(Vector2(lonlat_resolution/2,lonlat_resolution/2));
    TS_ASSERT_DELTA( point.x(), 0, 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1), 1e-5 );

    // South pole
    point = txform.forward(Vector2(3*lonlat_resolution/4,lonlat_resolution/2));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1), 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1), 1e-5 );

    // South pole
    point = txform.forward(Vector2(lonlat_resolution,lonlat_resolution/2));
    TS_ASSERT_DELTA( point.x(), (toast_resolution-1), 1e-5 );
    TS_ASSERT_DELTA( point.y(), (toast_resolution-1), 1e-5 );
  }

  void test_basic_reverse()
  {
    ToastTransform txform = get_basic_transform();
    Vector2 point;

    // Top left: (*,-90)
    point = txform.reverse(Vector2(0,0));
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/2, 1e-5 );

    // Top left edge: (-90,-45)
    point = txform.reverse(Vector2((toast_resolution-1)/4,0));
    TS_ASSERT_DELTA( point.x(), lonlat_resolution/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*lonlat_resolution/8, 1e-5 );

    // Top center: (-90,0)
    point = txform.reverse(Vector2((toast_resolution-1)/2,0));
    TS_ASSERT_DELTA( point.x(), lonlat_resolution/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/4, 1e-5 );

    // Top right edge: (-90,-45)
    point = txform.reverse(Vector2(3*(toast_resolution-1)/4,0));
    TS_ASSERT_DELTA( point.x(), lonlat_resolution/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*lonlat_resolution/8, 1e-5 );

    // Top right: (*,-90)
    point = txform.reverse(Vector2(toast_resolution-1,0));
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/2, 1e-5 );

    // Upper left edge: (0,-45)
    point = txform.reverse(Vector2(0,(toast_resolution-1)/4));
    TS_ASSERT_DELTA( point.x(), lonlat_resolution/2, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*lonlat_resolution/8, 1e-5 );

    // Upper left middle: (-45,0)
    point = txform.reverse(Vector2((toast_resolution-1)/4,(toast_resolution-1)/4));
    TS_ASSERT_DELTA( point.x(), 3*lonlat_resolution/8, 1e-5 );
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/4, 1e-5 );

    // Upper middle: (-90,45)
    point = txform.reverse(Vector2((toast_resolution-1)/2,(toast_resolution-1)/4));
    TS_ASSERT_DELTA( point.x(), lonlat_resolution/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/8, 1e-5 );

    // Upper right middle: (-135,0)
    point = txform.reverse(Vector2(3*(toast_resolution-1)/4,(toast_resolution-1)/4));
    TS_ASSERT_DELTA( point.x(), lonlat_resolution/8, 1e-5 );
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/4, 1e-5 );

    // Upper right edge: (-180,-45)
    point = txform.reverse(Vector2(toast_resolution-1,(toast_resolution-1)/4));
    TS_ASSERT_DELTA( point.x(), 0, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*lonlat_resolution/8, 1e-5 );

    // Left center: (0,0)
    point = txform.reverse(Vector2(0,(toast_resolution-1)/2));
    TS_ASSERT_DELTA( point.x(), lonlat_resolution/2, 1e-5 );
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/4, 1e-5 );

    // Left middle: (0,45)
    point = txform.reverse(Vector2((toast_resolution-1)/4,(toast_resolution-1)/2));
    TS_ASSERT_DELTA( point.x(), lonlat_resolution/2, 1e-5 );
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/8, 1e-5 );

    // Center: (*, 90)
    point = txform.reverse(Vector2((toast_resolution-1)/2,(toast_resolution-1)/2));
    TS_ASSERT_DELTA( point.y(), 0, 1e-5 );

    // Right middle: (-180,45)
    point = txform.reverse(Vector2(3*(toast_resolution-1)/4,(toast_resolution-1)/2));
    TS_ASSERT_DELTA( point.x(), 0, 1e-5 );
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/8, 1e-5 );

    // Right center: (-180,0)
    point = txform.reverse(Vector2(toast_resolution-1,(toast_resolution-1)/2));
    TS_ASSERT_DELTA( point.x(), 0, 1e-5 );
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/4, 1e-5 );

    // Lower left edge: (0,-45)
    point = txform.reverse(Vector2(0,3*(toast_resolution-1)/4));
    TS_ASSERT_DELTA( point.x(), lonlat_resolution/2, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*lonlat_resolution/8, 1e-5 );

    // Lower left middle: (45,0)
    point = txform.reverse(Vector2((toast_resolution-1)/4,3*(toast_resolution-1)/4));
    TS_ASSERT_DELTA( point.x(), 5*lonlat_resolution/8, 1e-5 );
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/4, 1e-5 );

    // Lower middle: (90,45)
    point = txform.reverse(Vector2((toast_resolution-1)/2,3*(toast_resolution-1)/4));
    TS_ASSERT_DELTA( point.x(), 3*lonlat_resolution/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/8, 1e-5 );

    // Lower right middle: (135,0)
    point = txform.reverse(Vector2(3*(toast_resolution-1)/4,3*(toast_resolution-1)/4));
    TS_ASSERT_DELTA( point.x(), 7*lonlat_resolution/8, 1e-5 );
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/4, 1e-5 );

    // Lower right edge: (-180,-45)
    point = txform.reverse(Vector2(toast_resolution-1,3*(toast_resolution-1)/4));
    TS_ASSERT_DELTA( point.x(), lonlat_resolution, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*lonlat_resolution/8, 1e-5 );

    // Bottom left: (*,-90)
    point = txform.reverse(Vector2(0,toast_resolution-1));
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/2, 1e-5 );

    // Bottom left edge: (90,-45)
    point = txform.reverse(Vector2((toast_resolution-1)/4,toast_resolution-1));
    TS_ASSERT_DELTA( point.x(), 3*lonlat_resolution/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*lonlat_resolution/8, 1e-5 );

    // Bottom center: (90,0)
    point = txform.reverse(Vector2((toast_resolution-1)/2,toast_resolution-1));
    TS_ASSERT_DELTA( point.x(), 3*lonlat_resolution/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/4, 1e-5 );

    // Bottom right edge: (90,-45)
    point = txform.reverse(Vector2(3*(toast_resolution-1)/4,toast_resolution-1));
    TS_ASSERT_DELTA( point.x(), 3*lonlat_resolution/4, 1e-5 );
    TS_ASSERT_DELTA( point.y(), 3*lonlat_resolution/8, 1e-5 );

    // Bottom right: (*,-90)
    point = txform.reverse(Vector2(toast_resolution-1,toast_resolution-1));
    TS_ASSERT_DELTA( point.y(), lonlat_resolution/2, 1e-5 );
  }

  void test_random_forward_and_reverse()
  {
    ToastTransform txform = get_basic_transform();
    Vector2 point, lonlat;

    srand(time(0));
    rand();

    for( int i=0; i<1000; ++i ) {
      int32 x = 1+(double)(lonlat_resolution-1)*rand()/RAND_MAX;
      int32 y = 1+(double)(lonlat_resolution/2-1)*rand()/RAND_MAX;
      point = txform.forward(Vector2(x,y));
      lonlat = txform.reverse(point);
      TS_ASSERT_DELTA( lonlat.x(), x, 1e-5 );
      TS_ASSERT_DELTA( lonlat.y(), y, 1e-5 );
    }
  }

};
