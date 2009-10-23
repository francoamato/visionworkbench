// __BEGIN_LICENSE__
// Copyright (C) 2006-2009 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


// TestGeoTransform.h
#include <cxxtest/TestSuite.h>

#include <vw/Cartography/GeoReference.h>
#include <vw/Cartography/GeoTransform.h>

using namespace vw;
using namespace vw::cartography;

class TestGeoTransform : public CxxTest::TestSuite
{
public:

  void test_basic_transform()
  {
    GeoReference src_georef;
    src_georef.set_well_known_geogcs("WGS84");

    GeoReference dst_georef;
    dst_georef.set_well_known_geogcs("WGS84");

    GeoTransform geotx(src_georef,dst_georef);

    Vector2 fwd = geotx.forward(Vector2(25,25)),
            rev = geotx.reverse(Vector2(25,25));

    TS_ASSERT_DELTA(fwd(0), 25, 1e-16);
    TS_ASSERT_DELTA(fwd(1), 25, 1e-16);
    TS_ASSERT_DELTA(rev(0), 25, 1e-16);
    TS_ASSERT_DELTA(rev(1), 25, 1e-16);
  }

  void test_utm_far_zone() {
    // This tests for a bug where forward_bbox calls latlon_to_* for a latlon
    // that is invalid for a utm zone.

    std::vector<Vector2> utm(4);
    utm[0] = Vector2(419832.648, 5184829.285); // UL
    utm[1] = Vector2(419832.648, 5129329.285); // LL
    utm[2] = Vector2(469332.648, 5184829.285); // UR
    utm[3] = Vector2(469332.648, 5129329.285); // LR

    Vector2 size(3300,3700);

    GeoReference ll_georef, utm_georef;

    Matrix3x3 utm_map;
    utm_map(0,0) =  (utm[2][0] - utm[0][0]) / size(0);
    utm_map(1,1) = -(utm[0][1] - utm[1][1]) / size(1);
    utm_map(0,2) = utm[0][0];
    utm_map(1,2) = utm[0][1];
    utm_map(2,2) = 1;
    utm_georef.set_transform(utm_map);
    utm_georef.set_pixel_interpretation(GeoReference::PixelAsPoint);
    // pick one far away from the poles
    utm_georef.set_UTM(59, false); // 59S

    Matrix3x3 ll_map;
    ll_map(0,0) =  360.0;
    ll_map(1,1) = -180.0;
    ll_map(0,2) = -180;
    ll_map(1,2) = 90;
    ll_map(2,2) = 1;
    ll_georef.set_transform(ll_map);

    GeoTransform geotx(utm_georef, ll_georef);

    BBox2i bbox, bbox2;

    // This should work
    TS_ASSERT_THROWS_NOTHING(bbox = geotx.forward_bbox(BBox2i(0,0,size[0], size[1])));

    // This should throw a proj.4 error because it tries to look up a lonlat
    // that is outside the utm zone
    TS_ASSERT_THROWS_ASSERT(bbox2 = geotx.reverse_bbox(BBox2i(0,0,size[0], size[1])),
			    const ArgumentErr &e, TS_ASSERT(std::string(e.what()).find("Proj.4 error") != std::string::npos));
  }
};
