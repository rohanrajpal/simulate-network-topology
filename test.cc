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
//    #if 1
//        LogComponentEnable ("test", LOG_LEVEL_INFO);
//    #endif
    double prob_rate = 3000000;
    uint32_t packet_size = 2048 / 8;
    uint64_t channel_capacity = 50000000;
    uint32_t prob_nBytesInPacket = 200;

    // Set up some default values for the simulation.  Use the
//    Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (210));
//    Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("448kb/s"));

    CommandLine cmd;
    bool enableFlowMonitor = false;
    cmd.AddValue ("EnableMonitor", "Enable Flow Monitor", enableFlowMonitor);
    cmd.Parse (argc, argv);

    //Create nodes
    NS_LOG_INFO ("Create nodes.");
    NodeContainer c;
    c.Create (3);
    NodeContainer n0n1 = NodeContainer(c.Get(0), c.Get(1));
    NodeContainer n2n1 = NodeContainer(c.Get(2), c.Get(1));

    InternetStackHelper internet;
    internet.Install (c);

    NS_LOG_INFO ("Create channels.");
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute ("DataRate", DataRateValue(DataRate(channel_capacity)));
    p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));
    NetDeviceContainer d0d1 = p2p.Install (n0n1);
    NetDeviceContainer d2d1 = p2p.Install (n2n1);

    // Later, we add IP addresses.
    NS_LOG_INFO ("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i0i1 = ipv4.Assign (d0d1);

    ipv4.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer i2i1 = ipv4.Assign (d2d1);

    // Create router nodes, initialize routing database and set up the routing
    // tables in the nodes.
   Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    NS_LOG_INFO ("Create Applications.");
    uint16_t port = 9;
    OnOffHelper onoff ("ns3::UdpSocketFactory",
                       Address (InetSocketAddress (i2i1.GetAddress (0), port)));
//    onoff.SetConstantRate (DataRate ("448kb/s"));
    onoff.SetAttribute("DataRate", DataRateValue(channel_capacity));
    onoff.SetAttribute("PacketSize", UintegerValue(packet_size));
    // On
    char string_duration_on[300];
    double duration_on;
    duration_on = (double) ((packet_size) * 8.0 / (double) (channel_capacity));
    sprintf(string_duration_on, "ns3::ConstantRandomVariable[Constant=" "%64.62f" "]", duration_on);
    onoff.SetAttribute("OnTime", StringValue(string_duration_on));

    //off
    char string_duration_off[300];
    double mean_duration_off;
    mean_duration_off = (prob_nBytesInPacket + 30.0) * 8.0 / prob_rate - duration_on;
    sprintf(string_duration_off, "ns3::ExponentialRandomVariable[Mean=" "%64.62f" "]", mean_duration_off);
    onoff.SetAttribute("OffTime", StringValue(string_duration_off));


    ApplicationContainer apps = onoff.Install (c.Get (0));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

    PacketSinkHelper sink ("ns3::UdpSocketFactory",
                           Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
    apps = sink.Install (c.Get (2));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

    AsciiTraceHelper ascii;
    p2p.EnableAsciiAll (ascii.CreateFileStream ("test-routing.tr"));
    p2p.EnablePcapAll ("test-routing");

    AnimationInterface anim("test-routing.xml");
    anim.SetConstantPosition(n0n1.Get(0),10.0,10.0);
    anim.SetConstantPosition(n0n1.Get(1),20.0,20.0);
    anim.SetConstantPosition(n2n1.Get(0),30.0,30.0);

    NS_LOG_INFO ("Run Simulation.");
    Simulator::Stop (Seconds (11));
    Simulator::Run ();
    NS_LOG_INFO ("Done.");

    Simulator::Destroy ();
    return 0;
}