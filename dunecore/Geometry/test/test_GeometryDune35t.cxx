// test_GeometryDune35t.cxx

// David Adams
// OCtober 2016
//
// Test the DUNE 35t geometry including channel mapping.

#include "test_GeometryDune.cxx"

//**********************************************************************

void setExpectedValues(ExpectedValues& ev) {
  ev.gname ="dune35t_geo";
  ev.fullname = "dune35t4apa_v6";
  // Geometry counts.
  ev.ncry = 1;
  ev.ntpc = 8;
  ev.npla = 3;
  ev.napa = 4;
  ev.nrop = 4;
  // Signal type and view for each TPC plane.
  resize(ev.view, ev.ncry, ev.ntpc, ev.npla, geo::kUnknown);
  resize(ev.sigType, ev.ncry, ev.ntpc, ev.npla, geo::kMysteryType);
  for ( Index icry=0; icry<ev.ncry; ++icry ) {
    for ( Index itpc=0; itpc<ev.ntpc; ++itpc ) {
      for ( Index ipla=0; ipla<ev.npla; ++ipla ) {
        ev.sigType[icry][itpc][ipla] = (ipla < 2) ? geo::kInduction : geo::kCollection;
      }
      ev.view[icry][itpc][0] = geo::kU;
      ev.view[icry][itpc][1] = geo::kV;
      ev.view[icry][itpc][2] = geo::kZ;
    }
  }
  // Wire count for each TPC plane.
  resize(ev.nwirPerPlane, ev.ntpc, ev.npla, 0);
  ev.nwirPerPlane[0][0] = 359;
  ev.nwirPerPlane[0][1] = 345;
  ev.nwirPerPlane[0][2] = 112;
  ev.nwirPerPlane[2][0] = 194;
  ev.nwirPerPlane[2][1] = 188;
  ev.nwirPerPlane[2][2] = 112;
  ev.nwirPerPlane[4][0] = 236;
  ev.nwirPerPlane[4][1] = 228;
  ev.nwirPerPlane[4][2] = 112;
  for ( Index ipla=0; ipla<ev.npla; ++ipla ) ev.nwirPerPlane[1][ipla] = ev.nwirPerPlane[0][ipla];
  for ( Index ipla=0; ipla<ev.npla; ++ipla ) ev.nwirPerPlane[3][ipla] = ev.nwirPerPlane[2][ipla];
  for ( Index ipla=0; ipla<ev.npla; ++ipla ) ev.nwirPerPlane[5][ipla] = ev.nwirPerPlane[4][ipla];
  for ( Index ipla=0; ipla<ev.npla; ++ipla ) ev.nwirPerPlane[6][ipla] = ev.nwirPerPlane[0][ipla];
  for ( Index ipla=0; ipla<ev.npla; ++ipla ) ev.nwirPerPlane[7][ipla] = ev.nwirPerPlane[0][ipla];
  resize(ev.nchaPerRop, ev.nrop, 0);
  ev.nchaPerRop[0] = 144;
  ev.nchaPerRop[1] = 144;
  ev.nchaPerRop[2] = 112;
  ev.nchaPerRop[3] = 112;
  for ( Index irop=0; irop<ev.nrop; ++irop ) ev.nchaPerApa += ev.nchaPerRop[irop];
  ev.nchatot = ev.napa*ev.nchaPerApa;
  resize(ev.chacry, ev.nchatot, InvalidIndex);
  resize(ev.chaapa, ev.nchatot, InvalidIndex);
  resize(ev.charop, ev.nchatot, InvalidIndex);
  Index icha = 0;
  for ( Index icry=0; icry<ev.ncry; ++icry ) {
    for ( Index iapa=0; iapa<ev.napa; ++iapa ) {
      for ( Index irop=0; irop<ev.nrop; ++irop ) {
        for ( Index kcha=0; kcha<ev.nchaPerRop[irop]; ++kcha ) {
          ev.chacry[icha] = icry;
          ev.chaapa[icha] = iapa;
          ev.charop[icha] = irop;
          ++icha;
        }
      }
    }
  }
  resize(ev.firstchan, ev.ncry, ev.napa, ev.nrop, raw::InvalidChannelID);
  Index chan = 0;
  for ( Index icry=0; icry<ev.ncry; ++icry ) {
    for ( Index iapa=0; iapa<ev.napa; ++iapa ) {
      for ( Index irop=0; irop<ev.nrop; ++irop ) {
        ev.firstchan[icry][iapa][irop] = chan;
        chan += ev.nchaPerRop[irop];
      }
    }
  }
  #include "set35tSpacePoints.dat"
  // Optical detectors.
  ev.nopdet = 8;
  ev.nopdetcha.push_back( 8);
  ev.nopdetcha.push_back(12);
  ev.nopdetcha.push_back( 2);
  ev.nopdetcha.push_back(12);
  ev.nopdetcha.push_back( 8);
  ev.nopdetcha.push_back(12);
  ev.nopdetcha.push_back( 8);
  ev.nopdetcha.push_back(12);
  ev.nopcha = ev.nopdet*12;
  resize(ev.opdetcha, ev.nopdet, 12, 0);
  for ( Index iopt=0; iopt<ev.nopdet; ++iopt ) {
    Index icha = 12*iopt;
    for ( Index ioch=0; ioch<ev.nopdetcha[iopt]; ++ ioch ) {
      ev.opdetcha[iopt][ioch] = icha++;
    }
  }
}

//**********************************************************************

void setExpectedValuesSpacePoints(Geometry* pgeo) {
  const string myname = "setExpectedValuespacePoints: ";
  string ofname = "set35tSpacePoints.dat";
  cout << myname << "Writing " << ofname << endl;
  const CryostatGeo& crygeo = pgeo->Cryostat(CryostatID{0});
  geo::CryostatGeo::LocalPoint_t const origin{};
  auto const crypos = crygeo.toWorldCoords(origin);
  double cxlo = crypos.X() - crygeo.HalfWidth();
  double cxhi = crypos.X() + crygeo.HalfWidth();
  double cylo = crypos.Y() - crygeo.HalfHeight();
  double cyhi = crypos.Y() + crygeo.HalfHeight();
  double czlo = crypos.Z() - 0.5*crygeo.Length();
  double czhi = crypos.Z() + 0.5*crygeo.Length();
  cout << "Cryostat limits: "
       << "(" << cxlo << ", " << cylo << ", " << czlo << "), "
       << "(" << cxhi << ", " << cyhi << ", " << czhi << ")" << endl;
  vector<double> zfs = {0.2, 0.3, 0.4, 0.5, 0.6, 0.68 };
  vector<double> yfs = {0.2, 0.5, 0.8};
  vector<double> xfs = {0.1, 0.3, 0.6 };
  ofstream fout(ofname.c_str());
  for ( double zf : zfs ) {
    double z = czlo + zf*(czhi-czlo);
    for ( double yf : yfs ) {
      double y = cylo + yf*(cyhi-cylo);
      for ( double xf : xfs ) {
        double x = cxlo + xf*(cxhi-cxlo);
        geo::Point_t const xyz{x, y, z};
        TPCID tpcid = pgeo->FindTPCAtPosition(xyz);
        unsigned int itpc = tpcid.TPC;
        const TPCGeo& tpcgeo = pgeo->TPC(tpcid);
        unsigned int npla = tpcgeo.Nplanes();
        fout << "  ev.posXyz.push_back(SpacePoint(" << x << ", " << y << ", " << z << "));" << endl;
        for ( unsigned int ipla=0; ipla<npla; ++ipla ) {
          PlaneID plaid(tpcid, ipla);
          double xwire = pgeo->WireCoordinate(geo::Point_t{0, x, y}, plaid);
          fout << "  ev.posTpc.push_back(" << itpc << ");" << endl;
          fout << "  ev.posPla.push_back(" << ipla << ");" << endl;
          fout << "  ev.posWco.push_back(" << xwire << ");" << endl;
        }  // end loop over planes
      }  // end loop over x
    }  // end loop over y
  }  // end loop over z
}

//**********************************************************************
