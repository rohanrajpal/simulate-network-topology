// 3 node linear topology
//
// n0 --- n1 --- n2
// source  r   sink

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("test");

int
main (int argc, char *argv[])
{
    #if 1
        LogComponentEnable ("test", LOG_LEVEL_INFO);
    #endif


    double prob_rate = 3000000;
    uint32_t packet_size = 2048 / 8;
    uint64_t channel_capacity = 50000000;
    uint32_t prob_nBytesInPacket = 200;

    // Set up some default values for the simulation.  Use the
//Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (210));
//Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("448kb/s"));

    CommandLine cmd;
    bool enableFlowMonitor = true;
    cmd.AddValue ("EnableMonitor", "Enable Flow Monitor", enableFlowMonitor);
    cmd.Parse (argc, argv);

    //Create nodes
    NS_LOG_INFO ("Create nodes.");
    NodeContainer c;
    c.Create (4);
    NodeContainer n0n1 = NodeContainer(c.Get(0), c.Get(1));
    NodeContainer n2n1 = NodeContainer(c.Get(2), c.Get(1));
    NodeContainer n3n1 = NodeContainer(c.Get(3),c.Get(1));


    InternetStackHelper internet;
    internet.Install (c);

    NS_LOG_INFO ("Create channels.");
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute ("DataRate", DataRateValue(DataRate(channel_capacity)));
    p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));
        p2p.SetQueue ("ns3::DropTailQueue",
                      "MaxSize", StringValue ("1p"));

    NetDeviceContainer d0d1 = p2p.Install (n0n1);
    NetDeviceContainer d2d1 = p2p.Install (n2n1);
    NetDeviceContainer d3d1 = p2p.Install (n3n1);



    //Give errors
//    Ptr<RateErrorModel> em = CreateObject<RateErrorModel> ();
//    em->SetAttribute ("ErrorRate", DoubleValue (0.0001));
//    d0d1.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));



    // Later, we add IP addresses.
    NS_LOG_INFO ("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i0i1 = ipv4.Assign (d0d1);

    ipv4.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer i2i1 = ipv4.Assign (d2d1);

    ipv4.SetBase ("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer i3i1 = ipv4.Assign (d3d1);

    // Create router nodes, initialize routing database and set up the routing
    // tables in the nodes.
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    NS_LOG_INFO ("Create Applications.");
    uint16_t port = 9;
    OnOffHelper onoff ("ns3::UdpSocketFactory",
                       Address (InetSocketAddress (i2i1.GetAddress (0), port)));

    onoff.SetAttribute("DataRate", DataRateValue(channel_capacity/4));
    onoff.SetAttribute("PacketSize", UintegerValue(packet_size));
    // On(Constant)
    char string_duration_on[300];
    double duration_on;
    duration_on = (double) ((packet_size) * 8.0 / (double) (channel_capacity));
    sprintf(string_duration_on, "ns3::ConstantRandomVariable[Constant=" "%64.62f" "]", duration_on);
    onoff.SetAttribute("OnTime", StringValue(string_duration_on));

    //Off(Random)
    char string_duration_off[300];
    double mean_duration_off;
    mean_duration_off = (prob_nBytesInPacket + 30.0) * 8.0 / prob_rate - duration_on;
    mean_duration_off = 0.001;
    sprintf(string_duration_off, "ns3::ExponentialRandomVariable[Mean=" "%64.62f" "]", mean_duration_off);
    onoff.SetAttribute("OffTime", StringValue(string_duration_off));

    NS_LOG_INFO (1 / mean_duration_off);

    ApplicationContainer apps = onoff.Install (c.Get (0));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

    PacketSinkHelper sink ("ns3::UdpSocketFactory",
                           Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
    apps = sink.Install (c.Get (2));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

    //Create a similar flow from n0 to n3, starting at time 1.1 seconds
    onoff.SetAttribute ("Remote",
                        AddressValue (InetSocketAddress (i3i1.GetAddress (0), port)));
    apps = onoff.Install (c.Get (0));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

    //make node 3 as the sink
    apps = sink.Install (c.Get (3));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

    //TODO: Set attribute of OnOff helper if destination is to be changed before associating with app.

    //Modifying off for n3
    mean_duration_off = 0.001;
    sprintf(string_duration_off, "ns3::ExponentialRandomVariable[Mean=" "%64.62f" "]", mean_duration_off);
    onoff.SetAttribute("OffTime", StringValue(string_duration_off));

    //set n0 as sink
    onoff.SetAttribute ("Remote",
                        AddressValue (InetSocketAddress (i0i1.GetAddress (0), port)));

    //Linking apps with n3
    apps = onoff.Install (c.Get (3));
    apps.Start (Seconds (1));
    apps.Stop (Seconds (10.0));

    //Make node 0 as sink
    apps = sink.Install (c.Get (0));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

    //FlowMonitor

    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> Monitor = flowmon.InstallAll();
    Monitor->SetAttribute ("DelayBinWidth", DoubleValue(0.001));
    Monitor->SetAttribute ("JitterBinWidth",DoubleValue (0.001));


    AsciiTraceHelper ascii;
    p2p.EnableAsciiAll (ascii.CreateFileStream ("test-routing.tr"));
    p2p.EnablePcapAll ("test-routing");

    AnimationInterface anim("test-routing.xml");
    anim.SetConstantPosition(n0n1.Get(0),10.0,10.0);
    anim.SetConstantPosition(n0n1.Get(1),20.0,20.0);
    anim.SetConstantPosition(n2n1.Get(0),30.0,30.0);
    anim.SetConstantPosition(n3n1.Get(0),30.0,20.0);

    NS_LOG_INFO ("Run Simulation.");
    Simulator::Stop (Seconds (11));
    Simulator::Run ();
    NS_LOG_INFO ("Done.");
    Monitor->SerializeToXmlFile ("results.xml",true,true);

    Simulator::Destroy ();
    return 0;
}