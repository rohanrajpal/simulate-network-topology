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

NS_LOG_COMPONENT_DEFINE ("randomtest");

int main(int argc, char *argv[]){
    LogComponentEnable ("randomtest", LOG_LEVEL_INFO);

    CommandLine cmd;
    bool placeHolder = true;
    cmd.AddValue ("PlaceHolder", "PlaceHolder Value", placeHolder);
    cmd.Parse (argc, argv);

    NS_LOG_INFO ("Create nodes.");
    NodeContainer c;
    c.Create (5);

    NodeContainer n0n3 = NodeContainer(c.Get(0), c.Get(3));
    NodeContainer n1n3 = NodeContainer(c.Get(1), c.Get(3));
    NodeContainer n2n3 = NodeContainer(c.Get(2), c.Get(3));
    NodeContainer n4n3 = NodeContainer(c.Get(4), c.Get(3));

    InternetStackHelper internet;
    internet.Install (c);

    NS_LOG_INFO ("Create channels.");
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
    p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));

//    p2p.SetQueue ("ns3::DropTailQueue","MaxSize", StringValue ("1000p"));

    NetDeviceContainer d0d3 = p2p.Install (n0n3);
    NetDeviceContainer d1d3 = p2p.Install (n1n3);
    NetDeviceContainer d2d3 = p2p.Install (n2n3);
    p2p.SetDeviceAttribute ("DataRate", StringValue ("3Mbps"));
    NetDeviceContainer d4d3 = p2p.Install (n4n3);

    NS_LOG_INFO ("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;

    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i0i3 = ipv4.Assign (d0d3);

    ipv4.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer i1i3 = ipv4.Assign (d1d3);

    ipv4.SetBase ("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer i2i3 = ipv4.Assign (d2d3);

    ipv4.SetBase ("10.1.4.0", "255.255.255.0");
    Ipv4InterfaceContainer i4i3 = ipv4.Assign (d4d3);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    NS_LOG_INFO ("Create Applications.");
    uint16_t port = 9;

    OnOffHelper onoff ("ns3::UdpSocketFactory",
                       Address (InetSocketAddress (i4i3.GetAddress (0), port)));
    onoff.SetConstantRate (DataRate ("1Mbps"));
    onoff.SetAttribute("PacketSize", UintegerValue(2048));

    ApplicationContainer apps = onoff.Install (c.Get (0));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

    onoff.SetAttribute ("Remote",
                        AddressValue (InetSocketAddress (i4i3.GetAddress (0), port)));
    apps = onoff.Install (c.Get (1));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

    onoff.SetAttribute ("Remote",
                        AddressValue (InetSocketAddress (i4i3.GetAddress (0), port)));
    apps = onoff.Install (c.Get (2));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

    //n4 is the one and only sink
    PacketSinkHelper sink ("ns3::UdpSocketFactory",
                           Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
    apps = sink.Install (c.Get (4));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));


    //FlowMonitor
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> Monitor = flowmon.InstallAll();
    Monitor->SetAttribute ("DelayBinWidth", DoubleValue(0.001));
    Monitor->SetAttribute ("JitterBinWidth",DoubleValue (0.001));

    AsciiTraceHelper ascii;
    p2p.EnableAsciiAll (ascii.CreateFileStream ("randomtest-routing.tr"));
    p2p.EnablePcapAll ("randomtest-routing");

    AnimationInterface anim("randomtest-routing.xml");
    anim.SetConstantPosition(n0n3.Get(0),10.0,10.0);
    anim.SetConstantPosition(n1n3.Get(0),10.0,30.0);
    anim.SetConstantPosition(n2n3.Get(0),10.0,50.0);
    anim.SetConstantPosition(n2n3.Get(1),30.0,30.0);
    anim.SetConstantPosition(n4n3.Get(0),50.0,30.0);

    NS_LOG_INFO ("Run Simulation.");
    Simulator::Stop (Seconds (11));
    Simulator::Run ();
    NS_LOG_INFO ("Done.");

    Monitor->SerializeToXmlFile ("randomtest-results.xml",true,true);

    Simulator::Destroy ();
    return 0;
}