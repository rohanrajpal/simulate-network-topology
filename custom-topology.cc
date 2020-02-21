// 5 node topology
//
/*
           A
           |
           r1
        /       \
   E-r5           r2 - B
        \        /
        r4    r5
       /        \
      D          C


*/
// source  r   sink

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <map>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/ipv4-interface-container.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("st-pnt-topo");

int main (int argc, char *argv[]){
    LogComponentEnable ("st-pnt-topo", LOG_LEVEL_INFO);

    CommandLine cmd;
    bool placeHolder = true;
    cmd.AddValue ("PlaceHolder", "PlaceHolder Value", placeHolder);
    cmd.Parse (argc, argv);

    std::map<std::string,int> mp;
    mp["a"] = 0;
    mp["b"] = 1;
    mp["c"] = 2;
    mp["d"] = 3;
    mp["e"] = 4;
    mp["r1"] = 5;
    mp["r2"] = 6;
    mp["r3"] = 7;
    mp["r4"] = 8;
    mp["r5"] = 9;

    NodeContainer c;
    c.Create(10);

    //created node pairs for p2p to be made later
    NodeContainer ar1 = NodeContainer(c.Get(mp["a"]), c.Get(mp["r1"]));
    NodeContainer br2 = NodeContainer(c.Get(mp["b"]), c.Get(mp["r2"]));
    NodeContainer cr3 = NodeContainer(c.Get(mp["c"]), c.Get(mp["r3"]));
    NodeContainer dr4 = NodeContainer(c.Get(mp["d"]), c.Get(mp["r4"]));
    NodeContainer er5 = NodeContainer(c.Get(mp["e"]), c.Get(mp["r5"]));
    
    NodeContainer r1r2 = NodeContainer(c.Get(mp["r1"]),c.Get(mp["r2"]));
    NodeContainer r2r3 = NodeContainer(c.Get(mp["r2"]),c.Get(mp["r3"]));
    NodeContainer r3r4 = NodeContainer(c.Get(mp["r3"]),c.Get(mp["r4"]));
    NodeContainer r4r5 = NodeContainer(c.Get(mp["r4"]),c.Get(mp["r5"]));
    NodeContainer r5r1 = NodeContainer(c.Get(mp["r5"]),c.Get(mp["r1"]));


    InternetStackHelper internet;
    internet.Install (c);

    PointToPointHelper p2p;
    p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));

    p2p.SetQueue ("ns3::DropTailQueue","MaxSize", StringValue ("100p"));
    NetDeviceContainer container_arr;

    p2p.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
    NetDeviceContainer d_ar1 = p2p.Install (ar1);

    p2p.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
    NetDeviceContainer d_br2 = p2p.Install (br2);

    p2p.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
    NetDeviceContainer d_cr3 = p2p.Install (cr3);

    p2p.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
    NetDeviceContainer d_dr4 = p2p.Install (dr4);

    p2p.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
    NetDeviceContainer d_er5 = p2p.Install (er5);

    p2p.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
    NetDeviceContainer d_r1r2 = p2p.Install (r1r2);

    p2p.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
    NetDeviceContainer d_r2r3 = p2p.Install (r2r3);

    p2p.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
    NetDeviceContainer d_r3r4 = p2p.Install (r3r4);

    p2p.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
    NetDeviceContainer d_r4r5 = p2p.Install (r4r5);

    p2p.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
    NetDeviceContainer d_r5r1 = p2p.Install (r5r1);

    //Introducing error
    Ptr<RateErrorModel> em = CreateObject<RateErrorModel> ();
    em->SetAttribute ("ErrorRate", DoubleValue (0.000065));
    d_ar1.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
    d_br2.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
    d_cr3.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
    d_dr4.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
    d_er5.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));

    container_arr.Add(d_ar1.Get(0));
    container_arr.Add(d_br2.Get(0));
    container_arr.Add(d_cr3.Get(0));
    container_arr.Add(d_dr4.Get(0));
    container_arr.Add(d_er5.Get(0));
    container_arr.Add(d_r1r2.Get(0));
    container_arr.Add(d_r1r2.Get(1));
    container_arr.Add(d_r2r3.Get(0));
    container_arr.Add(d_r2r3.Get(1));
    container_arr.Add(d_r4r5.Get(0));
    container_arr.Add(d_r4r5.Get(1));

    Ipv4AddressHelper ipv4;

    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i_ar1 = ipv4.Assign (d_ar1);

    ipv4.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer i_br2 = ipv4.Assign (d_br2);

    ipv4.SetBase ("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer i_cr3 = ipv4.Assign (d_cr3);
    
    ipv4.SetBase ("10.1.4.0", "255.255.255.0");
    Ipv4InterfaceContainer i_dr4 = ipv4.Assign (d_dr4);

    ipv4.SetBase ("10.1.5.0", "255.255.255.0");
    Ipv4InterfaceContainer i_er5 = ipv4.Assign (d_er5);

    ipv4.SetBase ("10.1.6.0", "255.255.255.0");
    Ipv4InterfaceContainer i_r1r2 = ipv4.Assign (d_r1r2);

    ipv4.SetBase ("10.1.7.0", "255.255.255.0");
    Ipv4InterfaceContainer i_r2r3 = ipv4.Assign (d_r2r3);

    ipv4.SetBase ("10.1.8.0", "255.255.255.0");
    Ipv4InterfaceContainer i_r3r4 = ipv4.Assign (d_r3r4);

    ipv4.SetBase ("10.1.9.0", "255.255.255.0");
    Ipv4InterfaceContainer i_r4r5 = ipv4.Assign (d_r4r5);

    ipv4.SetBase ("10.1.10.0", "255.255.255.0");
    Ipv4InterfaceContainer i_r5r1 = ipv4.Assign (d_r5r1);

//    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    Ipv4StaticRoutingHelper ipv4RoutingHelper;

    Ptr<Ipv4> ipv4_a = c.Get(mp["a"])->GetObject<Ipv4>();
    Ptr<Ipv4> ipv4_b = c.Get(mp["b"])->GetObject<Ipv4>();
    Ptr<Ipv4> ipv4_c = c.Get(mp["c"])->GetObject<Ipv4>();
    Ptr<Ipv4> ipv4_d = c.Get(mp["d"])->GetObject<Ipv4>();
    Ptr<Ipv4> ipv4_e = c.Get(mp["e"])->GetObject<Ipv4>();

    Ptr<Ipv4> ipv4_r1 = c.Get(mp["r1"])->GetObject<Ipv4>();
    Ptr<Ipv4> ipv4_r2 = c.Get(mp["r2"])->GetObject<Ipv4>();
    Ptr<Ipv4> ipv4_r3 = c.Get(mp["r3"])->GetObject<Ipv4>();
    Ptr<Ipv4> ipv4_r4 = c.Get(mp["r4"])->GetObject<Ipv4>();
    Ptr<Ipv4> ipv4_r5 = c.Get(mp["r5"])->GetObject<Ipv4>();



    Ptr<Ipv4StaticRouting> staticRouting_a = ipv4RoutingHelper.GetStaticRouting(ipv4_a);
    Ptr<Ipv4StaticRouting> staticRouting_b = ipv4RoutingHelper.GetStaticRouting(ipv4_b);
    Ptr<Ipv4StaticRouting> staticRouting_c = ipv4RoutingHelper.GetStaticRouting(ipv4_c);
    Ptr<Ipv4StaticRouting> staticRouting_d = ipv4RoutingHelper.GetStaticRouting(ipv4_d);
    Ptr<Ipv4StaticRouting> staticRouting_e = ipv4RoutingHelper.GetStaticRouting(ipv4_e);

    Ptr<Ipv4StaticRouting> staticRouting_r1 = ipv4RoutingHelper.GetStaticRouting(ipv4_r1);
    Ptr<Ipv4StaticRouting> staticRouting_r2 = ipv4RoutingHelper.GetStaticRouting(ipv4_r2);
    Ptr<Ipv4StaticRouting> staticRouting_r3 = ipv4RoutingHelper.GetStaticRouting(ipv4_r3);
    Ptr<Ipv4StaticRouting> staticRouting_r4 = ipv4RoutingHelper.GetStaticRouting(ipv4_r4);
    Ptr<Ipv4StaticRouting> staticRouting_r5 = ipv4RoutingHelper.GetStaticRouting(ipv4_r5);



    //a to b
    staticRouting_a->AddHostRouteTo(i_br2.GetAddress(0),i_ar1.GetAddress(1),  ipv4_a->GetInterfaceForDevice (d_ar1.Get(0))    );
    staticRouting_r1->AddHostRouteTo(i_br2.GetAddress(0),i_r1r2.GetAddress(1),ipv4_r1->GetInterfaceForDevice (d_r1r2.Get(0)) );
    staticRouting_r2->AddHostRouteTo(i_br2.GetAddress(0),i_br2.GetAddress(0), ipv4_r2->GetInterfaceForDevice (d_br2.Get(1))   );
//    NS_LOG_INFO(i_ar1.Get(0).second);
    //a to c
    staticRouting_a->AddHostRouteTo(i_cr3.GetAddress(0),i_ar1.GetAddress(1),ipv4_a->GetInterfaceForDevice (d_ar1.Get(0)));
    staticRouting_r1->AddHostRouteTo(i_cr3.GetAddress(0),i_r1r2.GetAddress(1),ipv4_r1->GetInterfaceForDevice (d_r1r2.Get(0)));
    staticRouting_r2->AddHostRouteTo(i_cr3.GetAddress(0),i_r2r3.GetAddress(1),ipv4_r2->GetInterfaceForDevice (d_r2r3.Get(0)));
    staticRouting_r3->AddHostRouteTo(i_cr3.GetAddress(0),i_cr3.GetAddress(0),ipv4_r3->GetInterfaceForDevice (d_cr3.Get(1)));

    // a to d
    staticRouting_a->AddHostRouteTo(i_dr4.GetAddress(0),i_ar1.GetAddress(1),ipv4_a->GetInterfaceForDevice (d_ar1.Get(0)));
    staticRouting_r1->AddHostRouteTo(i_dr4.GetAddress(0),i_r5r1.GetAddress(0),ipv4_r1->GetInterfaceForDevice (d_r5r1.Get(1)));
    staticRouting_r5->AddHostRouteTo(i_dr4.GetAddress(0),i_r4r5.GetAddress(0),ipv4_r5->GetInterfaceForDevice (d_r4r5.Get(1)));
    staticRouting_r4->AddHostRouteTo(i_dr4.GetAddress(0),i_dr4.GetAddress(0),ipv4_r4->GetInterfaceForDevice (d_dr4.Get(1)));

    //a to e

    staticRouting_a->AddHostRouteTo(i_er5.GetAddress(0),i_ar1.GetAddress(1),ipv4_a->GetInterfaceForDevice (d_ar1.Get(0)));
    staticRouting_r1->AddHostRouteTo(i_er5.GetAddress(0),i_r5r1.GetAddress(0),ipv4_r1->GetInterfaceForDevice (d_r5r1.Get(1)));
    staticRouting_r5->AddHostRouteTo(i_er5.GetAddress(0),i_er5.GetAddress(0),ipv4_r5->GetInterfaceForDevice (d_er5.Get(1)));

    //b to a
    staticRouting_b->AddHostRouteTo( i_ar1.GetAddress(0),i_br2.GetAddress(1),ipv4_b->GetInterfaceForDevice (d_br2.Get(0)) );
    staticRouting_r2->AddHostRouteTo(i_ar1.GetAddress(0),i_r1r2.GetAddress(0),i_r1r2.Get(1).second );
    staticRouting_r1->AddHostRouteTo(i_ar1.GetAddress(0),i_ar1.GetAddress(0),i_ar1.Get(0).second);

    // //b to c
    staticRouting_b->AddHostRouteTo(i_cr3.GetAddress(0),i_br2.GetAddress(1),ipv4_b->GetInterfaceForDevice (d_br2.Get(0)));
    staticRouting_r2->AddHostRouteTo(i_cr3.GetAddress(0),i_r2r3.GetAddress(1),ipv4_r2->GetInterfaceForDevice (d_r2r3.Get(0)));
    staticRouting_r3->AddHostRouteTo(i_cr3.GetAddress(0),i_cr3.GetAddress(0),ipv4_r3->GetInterfaceForDevice (d_cr3.Get(1)));

    // //b to d
    staticRouting_b->AddHostRouteTo(i_dr4.GetAddress(0),i_br2.GetAddress(1),ipv4_b->GetInterfaceForDevice (d_br2.Get(0)));
    staticRouting_r2->AddHostRouteTo(i_dr4.GetAddress(0),i_r2r3.GetAddress(1),ipv4_r2->GetInterfaceForDevice (d_r2r3.Get(0)));
    staticRouting_r3->AddHostRouteTo(i_dr4.GetAddress(0),i_r3r4.GetAddress(1),ipv4_r3->GetInterfaceForDevice (d_r3r4.Get(0)));
    staticRouting_r4->AddHostRouteTo(i_dr4.GetAddress(0),i_dr4.GetAddress(0),ipv4_r4->GetInterfaceForDevice (d_dr4.Get(1)));

    //b to e
    staticRouting_b->AddHostRouteTo(i_er5.GetAddress(0),i_br2.GetAddress(1),ipv4_b->GetInterfaceForDevice (d_br2.Get(0)));
    staticRouting_r2->AddHostRouteTo(i_er5.GetAddress(0),i_r1r2.GetAddress(0),ipv4_r2->GetInterfaceForDevice (d_r1r2.Get(1)));
    staticRouting_r1->AddHostRouteTo(i_er5.GetAddress(0),i_r5r1.GetAddress(0),ipv4_r1->GetInterfaceForDevice (d_r5r1.Get(1)));
    staticRouting_r5->AddHostRouteTo(i_er5.GetAddress(0),i_er5.GetAddress(0),ipv4_r5->GetInterfaceForDevice (d_er5.Get(1)));

    // //c to a
    staticRouting_c->AddHostRouteTo(i_ar1.GetAddress(0),i_cr3.GetAddress(1),ipv4_c->GetInterfaceForDevice (d_cr3.Get(0)));
    staticRouting_r3->AddHostRouteTo(i_ar1.GetAddress(0),i_r2r3.GetAddress(0),ipv4_r3->GetInterfaceForDevice (d_r2r3.Get(1)));
    staticRouting_r2->AddHostRouteTo(i_ar1.GetAddress(0),i_r1r2.GetAddress(0),ipv4_r2->GetInterfaceForDevice (d_r1r2.Get(1)));
    staticRouting_r1->AddHostRouteTo(i_ar1.GetAddress(0),i_ar1.GetAddress(0),ipv4_r1->GetInterfaceForDevice (d_ar1.Get(1)));

    //c to b
    staticRouting_c->AddHostRouteTo(i_br2.GetAddress(0),i_cr3.GetAddress(1),ipv4_c->GetInterfaceForDevice (d_cr3.Get(0)));
    staticRouting_r3->AddHostRouteTo(i_br2.GetAddress(0),i_r2r3.GetAddress(0),ipv4_r3->GetInterfaceForDevice (d_r2r3.Get(1)));
    staticRouting_r2->AddHostRouteTo(i_br2.GetAddress(0),i_br2.GetAddress(0),ipv4_r2->GetInterfaceForDevice (d_br2.Get(1)));

    //c to d
    staticRouting_c->AddHostRouteTo(i_dr4.GetAddress(0),i_cr3.GetAddress(1),ipv4_c->GetInterfaceForDevice (d_cr3.Get(0)));
    staticRouting_r3->AddHostRouteTo(i_dr4.GetAddress(0),i_r3r4.GetAddress(1),ipv4_r3->GetInterfaceForDevice (d_r3r4.Get(0)));
    staticRouting_r4->AddHostRouteTo(i_dr4.GetAddress(0),i_dr4.GetAddress(0),ipv4_r4->GetInterfaceForDevice (d_dr4.Get(1)));

    //c to e
    staticRouting_c->AddHostRouteTo(i_er5.GetAddress(0),i_cr3.GetAddress(1),ipv4_c->GetInterfaceForDevice (d_cr3.Get(0)));
    staticRouting_r3->AddHostRouteTo(i_er5.GetAddress(0),i_r3r4.GetAddress(1),ipv4_r3->GetInterfaceForDevice (d_r3r4.Get(0)));
    staticRouting_r4->AddHostRouteTo(i_er5.GetAddress(0),i_r4r5.GetAddress(1),ipv4_r4->GetInterfaceForDevice (d_r4r5.Get(0)));
    staticRouting_r5->AddHostRouteTo(i_er5.GetAddress(0),i_er5.GetAddress(0),ipv4_r5->GetInterfaceForDevice (d_er5.Get(1)));

    //d to a
     staticRouting_d->AddHostRouteTo(i_ar1.GetAddress(0),i_dr4.GetAddress(1),ipv4_d->GetInterfaceForDevice (d_dr4.Get(0)));
     staticRouting_r4->AddHostRouteTo(i_ar1.GetAddress(0),i_r4r5.GetAddress(1),ipv4_r4->GetInterfaceForDevice (d_r4r5.Get(0)));
     staticRouting_r5->AddHostRouteTo(i_ar1.GetAddress(0),i_r5r1.GetAddress(1),ipv4_r5->GetInterfaceForDevice (d_r5r1.Get(0)));
     staticRouting_r1->AddHostRouteTo(i_ar1.GetAddress(0),i_ar1.GetAddress(0),ipv4_r1->GetInterfaceForDevice (d_ar1.Get(1)));

    // d to b
    staticRouting_d->AddHostRouteTo(i_br2.GetAddress(0),i_dr4.GetAddress(1),ipv4_d->GetInterfaceForDevice (d_dr4.Get(0)));
    staticRouting_r4->AddHostRouteTo(i_br2.GetAddress(0),i_r3r4.GetAddress(0),ipv4_r4->GetInterfaceForDevice (d_r3r4.Get(1)));
    staticRouting_r3->AddHostRouteTo(i_br2.GetAddress(0),i_r2r3.GetAddress(0),ipv4_r3->GetInterfaceForDevice (d_r2r3.Get(1)));
    staticRouting_r2->AddHostRouteTo(i_br2.GetAddress(0),i_r2r3.GetAddress(0),ipv4_r2->GetInterfaceForDevice (d_r2r3.Get(1)));


    //d to c
    staticRouting_d ->AddHostRouteTo(i_cr3.GetAddress(0),i_dr4.GetAddress(1),ipv4_d->GetInterfaceForDevice (d_dr4.Get(0)));
    staticRouting_r4->AddHostRouteTo(i_cr3.GetAddress(0),i_r3r4.GetAddress(0),ipv4_r4->GetInterfaceForDevice (d_r3r4.Get(1)));
    staticRouting_r3->AddHostRouteTo(i_cr3.GetAddress(0),i_cr3.GetAddress(0),ipv4_r3->GetInterfaceForDevice (d_cr3.Get(1)));

    //d to e
    staticRouting_d ->AddHostRouteTo(i_er5.GetAddress(0),i_dr4.GetAddress(1),ipv4_d->GetInterfaceForDevice (d_dr4.Get(0)));
    staticRouting_r4->AddHostRouteTo(i_er5.GetAddress(0),i_r4r5.GetAddress(1),ipv4_r4->GetInterfaceForDevice (d_r4r5.Get(0)));
    staticRouting_r5->AddHostRouteTo(i_er5.GetAddress(0),i_er5.GetAddress(0),ipv4_r5->GetInterfaceForDevice (d_er5.Get(1)));

    //e to a
    staticRouting_e->AddHostRouteTo(i_ar1.GetAddress(0),i_er5.GetAddress(1),ipv4_e->GetInterfaceForDevice (d_er5.Get(0)));
    staticRouting_r5->AddHostRouteTo(i_ar1.GetAddress(0),i_r5r1.GetAddress(1),ipv4_r5->GetInterfaceForDevice (d_r5r1.Get(0)));
    staticRouting_r1->AddHostRouteTo(i_ar1.GetAddress(0),i_ar1.GetAddress(0),ipv4_r1->GetInterfaceForDevice (d_ar1.Get(1)));

    //e to b
    staticRouting_e->AddHostRouteTo(i_br2.GetAddress(0),i_er5.GetAddress(1),ipv4_e->GetInterfaceForDevice (d_er5.Get(0)));
    staticRouting_r5->AddHostRouteTo(i_br2.GetAddress(0),i_r5r1.GetAddress(1),ipv4_r5->GetInterfaceForDevice (d_r5r1.Get(0)));
    staticRouting_r1->AddHostRouteTo(i_br2.GetAddress(0),i_r1r2.GetAddress(1),ipv4_r1->GetInterfaceForDevice (d_r1r2.Get(0)));
    staticRouting_r2->AddHostRouteTo(i_br2.GetAddress(0),i_br2.GetAddress(0),ipv4_r2->GetInterfaceForDevice (d_br2.Get(1)));

    // e to c
    staticRouting_e->AddHostRouteTo(i_cr3.GetAddress(0),i_er5.GetAddress(1),ipv4_e->GetInterfaceForDevice (d_er5.Get(0)));
    staticRouting_r5->AddHostRouteTo(i_cr3.GetAddress(0),i_r4r5.GetAddress(0),ipv4_r5->GetInterfaceForDevice (d_r4r5.Get(1)));
    staticRouting_r4->AddHostRouteTo(i_cr3.GetAddress(0),i_r3r4.GetAddress(0),ipv4_r4->GetInterfaceForDevice (d_r3r4.Get(1)));
    staticRouting_r3->AddHostRouteTo(i_cr3.GetAddress(0),i_cr3.GetAddress(0),ipv4_r3->GetInterfaceForDevice (d_cr3.Get(1)));

    // e to d
    staticRouting_e->AddHostRouteTo(i_dr4.GetAddress(0),i_er5.GetAddress(1),ipv4_e->GetInterfaceForDevice (d_er5.Get(0)));
    staticRouting_r5->AddHostRouteTo(i_dr4.GetAddress(0),i_r4r5.GetAddress(0),ipv4_r5->GetInterfaceForDevice (d_r4r5.Get(1)));
    staticRouting_r4->AddHostRouteTo(i_dr4.GetAddress(0),i_dr4.GetAddress(0),ipv4_r4->GetInterfaceForDevice (d_dr4.Get(1)));

    uint16_t port = 9;

    char string_duration_on[300];
    char string_duration_off[300];


    double duration_on_A = 0.000256; // Use rate 1 Mbps
    sprintf(string_duration_on, "ns3::ConstantRandomVariable[Constant=" "%64.62f" "]", duration_on_A);

    double mean_duration_off_A = 0.009748013842179656;
    sprintf(string_duration_off, "ns3::ExponentialRandomVariable[Mean=" "%64.62f" "]", mean_duration_off_A);

//    A to D
    OnOffHelper onoff1 ("ns3::UdpSocketFactory", Address (InetSocketAddress (i_dr4.GetAddress (0), port)));
//    A to C
    OnOffHelper onoff2 ("ns3::UdpSocketFactory",Address (InetSocketAddress (i_cr3.GetAddress (0), port)));
    //A to B
    OnOffHelper onoff3 ("ns3::UdpSocketFactory",Address (InetSocketAddress (i_br2.GetAddress (0), port)));
    //A to E
    OnOffHelper onoff4 ("ns3::UdpSocketFactory",Address (InetSocketAddress (i_er5.GetAddress (0), port)));

    //For sending from A to D
    onoff1.SetAttribute("PacketSize", UintegerValue(256));
    onoff1.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff1.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff1.SetAttribute("DataRate", StringValue ("1Mbps"));

    ApplicationContainer apps = onoff1.Install (c.Get (mp["a"]));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (61.0));

    //For sending from A to C
    onoff2.SetAttribute("PacketSize", UintegerValue(256));
    onoff2.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff2.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff2.SetAttribute("DataRate", StringValue ("1Mbps"));
//
    onoff1.SetAttribute("Remote", AddressValue (InetSocketAddress (i_cr3.GetAddress (0), port)));

    apps = onoff1.Install (c.Get (mp["a"]));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (61.0));

//     A to B
    onoff3.SetAttribute("PacketSize", UintegerValue(256));
    onoff3.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff3.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff3.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps = onoff3.Install (c.Get (mp["a"]));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (61.0));

    // A to E
    onoff4.SetAttribute("PacketSize", UintegerValue(256));
    onoff4.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff4.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff4.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps = onoff4.Install (c.Get (mp["a"]));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (61.0));

    //B is source

    //B to E
    OnOffHelper onoff_b1("ns3::UdpSocketFactory",Address (InetSocketAddress (i_er5.GetAddress (0), port)));
    //B to A
    OnOffHelper onoff_b2("ns3::UdpSocketFactory",Address (InetSocketAddress (i_ar1.GetAddress (0), port)));
    //B to c
    OnOffHelper onoff_b3("ns3::UdpSocketFactory",Address (InetSocketAddress (i_cr3.GetAddress (0), port)));
    //B to D
    OnOffHelper onoff_b4("ns3::UdpSocketFactory",Address (InetSocketAddress (i_dr4.GetAddress (0), port)));

    //For sending from B to E
    onoff_b1.SetAttribute("PacketSize", UintegerValue(256));
    onoff_b1.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_b1.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_b1.SetAttribute("DataRate", StringValue ("1Mbps"));

    onoff_b1.SetAttribute("Remote", AddressValue (InetSocketAddress (i_ar1.GetAddress (0), port)));
    apps = onoff_b1.Install (c.Get (mp["b"]));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (61.0));

//    For sending from B to A
    onoff_b2.SetAttribute("PacketSize", UintegerValue(256));
    onoff_b2.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_b2.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_b2.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps = onoff_b1.Install (c.Get (mp["b"]));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (61.0));

    //For sending from B to C
    onoff_b3.SetAttribute("PacketSize", UintegerValue(256));
    onoff_b3.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_b3.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_b3.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps= onoff_b3.Install (c.Get (mp["b"]));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (61.0));

    //For sending from B to D
    onoff_b4.SetAttribute("PacketSize", UintegerValue(256));
    onoff_b4.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_b4.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_b4.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps= onoff_b4.Install (c.Get (mp["b"]));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (61.0));

    //Setting up sinks at D and E
    PacketSinkHelper sink ("ns3::UdpSocketFactory",Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
    PacketSinkHelper sink2 ("ns3::UdpSocketFactory",Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
    PacketSinkHelper sink3 ("ns3::UdpSocketFactory",Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
    PacketSinkHelper sink4 ("ns3::UdpSocketFactory",Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
    PacketSinkHelper sink5 ("ns3::UdpSocketFactory",Address (InetSocketAddress (Ipv4Address::GetAny (), port)));

    //C is source

    //C to E
    OnOffHelper onoff_c1("ns3::UdpSocketFactory",Address (InetSocketAddress (i_er5.GetAddress (0), port)));
    //C to A
    OnOffHelper onoff_c2("ns3::UdpSocketFactory",Address (InetSocketAddress (i_ar1.GetAddress (0), port)));
    //C to B
    OnOffHelper onoff_c3("ns3::UdpSocketFactory",Address (InetSocketAddress (i_br2.GetAddress (0), port)));
    //C to D
    OnOffHelper onoff_c4("ns3::UdpSocketFactory",Address (InetSocketAddress (i_dr4.GetAddress (0), port)));

    //For sending from C to E
    onoff_c1.SetAttribute("PacketSize", UintegerValue(256));
    onoff_c1.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_c1.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_c1.SetAttribute("DataRate", StringValue ("1Mbps"));
    ApplicationContainer apps_c1= onoff_c1.Install (c.Get (mp["c"]));
    apps_c1.Start (Seconds (1.0));
    apps_c1.Stop (Seconds (61.0));

    //For sending from C to A
    onoff_c2.SetAttribute("PacketSize", UintegerValue(256));
    onoff_c2.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_c2.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_c2.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps_c1= onoff_c2.Install (c.Get (mp["c"]));
    apps_c1.Start (Seconds (1.0));
    apps_c1.Stop (Seconds (61.0));

    //For sending from C to B
    onoff_c3.SetAttribute("PacketSize", UintegerValue(256));
    onoff_c3.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_c3.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_c3.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps_c1= onoff_c3.Install (c.Get (mp["c"]));
    apps_c1.Start (Seconds (1.0));
    apps_c1.Stop (Seconds (61.0));

    //For sending from C to D
    onoff_c4.SetAttribute("PacketSize", UintegerValue(256));
    onoff_c4.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_c4.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_c4.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps_c1= onoff_c4.Install (c.Get (mp["c"]));
    apps_c1.Start (Seconds (1.0));
    apps_c1.Stop (Seconds (61.0));

    //D is source

    //D to E
    OnOffHelper onoff_d1("ns3::UdpSocketFactory",Address (InetSocketAddress (i_er5.GetAddress (0), port)));
    //D to A
    OnOffHelper onoff_d2("ns3::UdpSocketFactory",Address (InetSocketAddress (i_ar1.GetAddress (0), port)));
    //D to B
    OnOffHelper onoff_d3("ns3::UdpSocketFactory",Address (InetSocketAddress (i_br2.GetAddress (0), port)));
    //D to C
    OnOffHelper onoff_d4("ns3::UdpSocketFactory",Address (InetSocketAddress (i_cr3.GetAddress (0), port)));

    //For sending from D to E
    onoff_d1.SetAttribute("PacketSize", UintegerValue(256));
    onoff_d1.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_d1.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_d1.SetAttribute("DataRate", StringValue ("1Mbps"));
    ApplicationContainer apps_d1= onoff_d1.Install (c.Get (mp["d"]));
    apps_d1.Start (Seconds (1.0));
    apps_d1.Stop (Seconds (61.0));

    //For sending from D to A
    onoff_d2.SetAttribute("PacketSize", UintegerValue(256));
    onoff_d2.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_d2.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_d2.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps_d1= onoff_d2.Install (c.Get (mp["d"]));
    apps_d1.Start (Seconds (1.0));
    apps_d1.Stop (Seconds (61.0));

    //For sending from D to B
    onoff_d3.SetAttribute("PacketSize", UintegerValue(256));
    onoff_d3.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_d3.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_d3.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps_d1= onoff_d3.Install (c.Get (mp["d"]));
    apps_d1.Start (Seconds (1.0));
    apps_d1.Stop (Seconds (61.0));

    //For sending from D to C
    onoff_d4.SetAttribute("PacketSize", UintegerValue(256));
    onoff_d4.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_d4.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_d4.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps_d1= onoff_d4.Install (c.Get (mp["d"]));
    apps_d1.Start (Seconds (1.0));
    apps_d1.Stop (Seconds (61.0));

    //E is source

    //E to D
    OnOffHelper onoff_e1("ns3::UdpSocketFactory",Address (InetSocketAddress (i_dr4.GetAddress (0), port)));
    //E to A
    OnOffHelper onoff_e2("ns3::UdpSocketFactory",Address (InetSocketAddress (i_ar1.GetAddress (0), port)));
    //E to B
    OnOffHelper onoff_e3("ns3::UdpSocketFactory",Address (InetSocketAddress (i_br2.GetAddress (0), port)));
    //E to C
    OnOffHelper onoff_e4("ns3::UdpSocketFactory",Address (InetSocketAddress (i_cr3.GetAddress (0), port)));

    //For sending from E to D
    onoff_e1.SetAttribute("PacketSize", UintegerValue(256));
    onoff_e1.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_e1.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_e1.SetAttribute("DataRate", StringValue ("1Mbps"));
    ApplicationContainer apps_e1= onoff_e1.Install (c.Get (mp["e"]));
    apps_e1.Start (Seconds (1.0));
    apps_e1.Stop (Seconds (61.0));

    //For sending from E to A
    onoff_e2.SetAttribute("PacketSize", UintegerValue(256));
    onoff_e2.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_e2.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_e2.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps_e1= onoff_e2.Install (c.Get (mp["e"]));
    apps_e1.Start (Seconds (1.0));
    apps_e1.Stop (Seconds (61.0));

    //For sending from E to B
    onoff_e3.SetAttribute("PacketSize", UintegerValue(256));
    onoff_e3.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_e3.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_e3.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps_e1= onoff_e3.Install (c.Get (mp["e"]));
    apps_e1.Start (Seconds (1.0));
    apps_e1.Stop (Seconds (61.0));

    //For sending from E to C
    onoff_e4.SetAttribute("PacketSize", UintegerValue(256));
    onoff_e4.SetAttribute("OnTime", StringValue(string_duration_on));
    onoff_e4.SetAttribute("OffTime", StringValue(string_duration_off));
    onoff_e4.SetAttribute("DataRate", StringValue ("1Mbps"));
    apps_e1= onoff_e4.Install (c.Get (mp["e"]));
    apps_e1.Start (Seconds (1.0));
    apps_e1.Stop (Seconds (61.0));

    apps = sink.Install (c.Get (mp["a"]));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (61.0));

    apps = sink2.Install (c.Get (mp["b"]));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (61.0));

    apps = sink3.Install (c.Get (mp["c"]));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (61.0));

    apps = sink4.Install (c.Get (mp["d"]));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (61.0));

    apps = sink5.Install (c.Get (mp["e"]));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (61.0));




    //FlowMonitor
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> Monitor = flowmon.InstallAll();
    Monitor->SetAttribute ("DelayBinWidth", DoubleValue(0.001));
    Monitor->SetAttribute ("JitterBinWidth",DoubleValue (0.001));

    AsciiTraceHelper ascii;
    p2p.EnableAsciiAll (ascii.CreateFileStream ("st-pnt-topo.tr"));
    p2p.EnablePcapAll ("st-pnt-topo");

    // AsciiTraceHelper ascii;
    // p2p.EnableAsciiAll (ascii.CreateFileStream ("queuetest-routing.tr"));
    // p2p.EnablePcapAll ("queuetest-routing");

    AnimationInterface anim("test-routing.xml");
    anim.SetConstantPosition(ar1.Get(0),80.0,60.0);
    anim.SetConstantPosition(br2.Get(0),120.0,80.0);
    anim.SetConstantPosition(cr3.Get(0),120.0,140.0);
    anim.SetConstantPosition(dr4.Get(0),40.0,140);
    anim.SetConstantPosition(er5.Get(0),40.0,80.0);
    anim.SetConstantPosition(r1r2.Get(0),80.0,80.0);
    anim.SetConstantPosition(r2r3.Get(0),100.0,100.0);
    anim.SetConstantPosition(r3r4.Get(0),100.0,120.0);
    anim.SetConstantPosition(r4r5.Get(0),60.0,120.0);
    anim.SetConstantPosition(r5r1.Get(0),60.0,100.0);


    NS_LOG_INFO ("Run Simulation.");
    Simulator::Stop (Seconds (61));
    Simulator::Run ();
    NS_LOG_INFO ("Done.");

    Monitor->SerializeToXmlFile ("st-pnt-topo.xml",false,false);

    Simulator::Destroy ();
    return 0;
}