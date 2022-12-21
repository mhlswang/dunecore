////////////////////////////////////////////////////////////////////////
// Class:       CheckCRPGeometry
// Module Type: analyzer
// File:        CheckCRPGeometry_module.cc
//
// Use: 
//     To debug / check geometry of VD CRP TPCs
// 
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "larcore/Geometry/Geometry.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TH2F.h"
#include "TLine.h"

#include <iostream>
#include <iomanip>

class CheckCRPGeometry;

class CheckCRPGeometry : public art::EDAnalyzer {
public:
  explicit CheckCRPGeometry(fhicl::ParameterSet const & p);
  // The destructor generated by the compiler is fine for classes
  // without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  CheckCRPGeometry(CheckCRPGeometry const &) = delete;
  CheckCRPGeometry(CheckCRPGeometry &&) = delete;
  CheckCRPGeometry & operator = (CheckCRPGeometry const &) = delete;
  CheckCRPGeometry & operator = (CheckCRPGeometry &&) = delete;

  // Required functions.
  void analyze(art::Event const & e) override;


private:

  // Declare member data here.
  //const double dpwpitch = 0.3125; //cm
  bool m_dump_wires;
};


CheckCRPGeometry::CheckCRPGeometry(fhicl::ParameterSet const & p)
  :
  EDAnalyzer(p), m_dump_wires(p.get< bool >("DumpWires", false))
 // More initializers here.
{;}


void CheckCRPGeometry::analyze(art::Event const & e)
{
  std::vector<TBox*> TPCBox;
  std::vector<TLine*> Wires;

  double minx = 1e9;
  double maxx = -1e9;
  double miny = 1e9;
  double maxy = -1e9;
  double minz = 1e9;
  double maxz = -1e9;
  
  int nwires = 0;

  // get geometry
  art::ServiceHandle<geo::Geometry> geo;
  /*
  // check channel map
  for( unsigned ch = 0; ch < geo->Nchannels(); ++ch ){
    auto wids = geo->ChannelToWire(ch);
    //std::cout<<ch<<" -> wids "<<wids.size()<<std::endl;
    std::cout<<"Channel to TPC "<< ch <<" "<<wids[0].TPC<<" "<<wids[0].Plane<<" "<<wids[0].Wire<<std::endl;
  }
  //return;
  */
  std::cout<<"Total number of TPC "<<geo->NTPC()<<std::endl;

  for (geo::TPCGeo const& tpc: geo->Iterate<geo::TPCGeo>(geo::CryostatID{0})) {
    size_t const t = tpc.ID().TPC;
    //if (t%2==0) continue;
    auto const world = tpc.GetCenter();
    if (minx>world.X()-tpc.ActiveHalfWidth())
      minx = world.X()-tpc.ActiveHalfWidth();
    if (maxx<world.X()+tpc.ActiveHalfWidth())
      maxx = world.X()+tpc.ActiveHalfWidth();
    if (miny>world.Y()-tpc.ActiveHalfHeight())
      miny = world.Y()-tpc.ActiveHalfHeight();
    if (maxy<world.Y()+tpc.ActiveHalfHeight())
      maxy = world.Y()+tpc.ActiveHalfHeight();
    if (minz>world.Z()-tpc.ActiveLength()/2.)
      minz = world.Z()-tpc.ActiveLength()/2.;
    if (maxz<world.Z()+tpc.ActiveLength()/2.)
      maxz = world.Z()+tpc.ActiveLength()/2.;


    TPCBox.push_back(new TBox(world.Z()-tpc.ActiveLength()/2.,
                              world.Y()-tpc.ActiveHalfHeight(),
                              world.Z()+tpc.ActiveLength()/2.,
                              world.Y()+tpc.ActiveHalfHeight()));
    TPCBox.back()->SetFillStyle(0);
    TPCBox.back()->SetLineStyle(2);
    TPCBox.back()->SetLineWidth(2);
    TPCBox.back()->SetLineColor(16);

    // std::cout<<"TPC "<<t<<" has found "<<geo->Nplanes(t)<<" planes"<<std::endl;
    // std::cout<<"TPC coordinates : "<<world.X()<<" "<<world.Y()<<" "<<world.Z()<<std::endl;
    // std::cout<<"Drift direction : ";
    // if(tpc.DriftDirection() == geo::kPosX) std::cout<<"geo::kPosX"<<std::endl;
    // else if(tpc.DriftDirection() == geo::kNegX) std::cout<<"geo::kNegX"<<std::endl;
    // else std::cout<<" Uknown drift direction"<<std::endl;
    // std::cout<<"Drift distance  : "<<tpc.DriftDistance()<<std::endl;
    std::cout<<tpc.TPCInfo("  ", 4)<<std::endl;
    std::cout<<std::endl;
    // scan the planes
    for (auto const& vPlane : geo->Iterate<geo::PlaneGeo>(tpc.ID()))
      {
        auto const& planeID = vPlane.ID();
	auto view = vPlane.View();
	if( view == geo::kU )
	  std::cout<<"  View type geo::kU"<<std::endl;
	else if( view == geo::kV )
	  std::cout<<"  View type geo::kV"<<std::endl;
	else if( view == geo::kX )
	  std::cout<<"  View type geo::kX"<<std::endl;
	else if( view == geo::kY )
	  std::cout<<"  View type geo::kY"<<std::endl;
	else if( view == geo::kZ )
	  std::cout<<"  View type geo::kZ"<<std::endl;
	else 
	  std::cout<<"  View "<<view<<" uknown"<<std::endl;
	
	auto sigtype = geo->SignalType(planeID);
	if(  sigtype == geo::kCollection )
	  std::cout<<"  View is geo::kCollection"<<std::endl;
	else if( sigtype == geo::kInduction )
	  std::cout<<"  View is geo::kInduction"<<std::endl;
	else
	  std::cout<<"  View signal type is unknown"<<std::endl;
	
	std::cout<<"  Number of wires : "<<vPlane.Nwires()<<std::endl;
	std::cout<<"  Wire pitch      : "<<vPlane.WirePitch()<<std::endl;
	std::cout<<"  Theta Z         : "<<vPlane.ThetaZ()<<std::endl;

	double prval    = 0; 
	double refpitch = 0;
        for (geo::WireID const& wid : geo->Iterate<geo::WireID>(planeID)) {
          auto const [p, w] = std::make_pair(wid.Plane, wid.Wire);
	  ++nwires;
	  //++nwires_tpc[t];
	  
	  if(true)
	    {
              double xyz0[3];
              double xyz1[3];
              geo->WireEndPoints(wid,xyz0,xyz1);
	      Wires.push_back(new TLine(xyz0[2],xyz0[1],xyz1[2],xyz1[1]));
	      
	      double pitch = 0;
	      //if( p == 0) {pitch = xyz0[1] - prval; prval = xyz0[1];}
	      //else if( p == 1) {pitch = xyz0[2] - prval; prval = xyz0[2];}
	      if( view == geo::kX )   {pitch = xyz0[0] - prval; prval = xyz0[0];}
	      else if(view == geo::kY){pitch = xyz0[1] - prval; prval = xyz0[1];}
	      else if(view == geo::kZ){pitch = xyz0[2] - prval; prval = xyz0[2];}
	      else { 
		continue;
		//std::cerr<<"cannot determine the view\n"; break;
	      }
	      if( w == 1 ){ refpitch = pitch; }
	      if(w > 0 && fabs(pitch - refpitch) > 0.00001)
		{
		  std::cerr<<" Bad pitch : "<<t<<" "<<p<<" "<<w<<" "<<w-1<<" "<<pitch<<" "<<refpitch<<std::endl;
		  //<<std::setprecision(15)<<xyz0[0]<<" "<<xyz0[1]<<" "<<xyz0[2]<<std::endl;
		}
	    }
	  //std::cout<<t<<" "<<p<<" "<<w<<" "<<xyz0[0]<<" "<<xyz0[1]<<" "<<xyz0[2]<<std::endl;
	}
	std::cout<<std::endl;
      }
    //break;
  }

  if( m_dump_wires ){
    
    double xyz[3];   
    double abc[3];                                                                 
    for (auto const& wid : geo->Iterate<geo::WireID>()) {
      geo->WireEndPoints(wid,xyz,abc);
      auto chan=geo->PlaneWireToChannel(wid);
      std::cout << "FLAG " << chan << " " << wid << " " << xyz[0] << " " << xyz[1] << " " << xyz[2] <<  " " << abc[0] << " " << abc[1] << " " << abc[2] << std::endl;
    }
  }// dump wires

    /*
  TCanvas *can = new TCanvas("c1","c1");
  can->cd();
  TH2F *frame = new TH2F("frame",";z (cm);y (cm)",3000,minz,maxz,3000,miny,maxy);
  frame->SetStats(0);
  frame->Draw();
  for (auto box: TPCBox) box->Draw();
  for (auto wire: Wires) wire->Draw();
  can->Print("wires.pdf");
  can->Print("wires.C");
  std::cout<<"N wires = "<<nwires<<std::endl;
  std::cout<<"Total number of channel wires = "<<nwires<<std::endl;
  //for (int i = 0; i<8; ++i)
  //{
  //std::cout<<"TPC "<<i<<" has "<<nwires_tpc[i]<<" wires"<<std::endl;
  //}
  */
}

DEFINE_ART_MODULE(CheckCRPGeometry)
