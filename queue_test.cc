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

int main(int argc, char *argv[]){
    CommandLine cmd;
    bool placeHolder = true;
    cmd.AddValue ("PlaceHolder", "PlaceHolder Value", placeHolder);
    cmd.Parse (argc, argv);

    NodeContainer c;
    c.Create (5);

    NodeContainer n0n1 = NodeContainer(c.Get(0), c.Get(1));
    NodeContainer n2n1 = NodeContainer(c.Get(2), c.Get(1));

    InternetStackHelper internet;
    internet.Install (c);

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
    p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));

    p2p.SetQueue ("ns3::DropTailQueue","MaxSize", StringValue ("10p"));

    NetDeviceContainer d0d1 = p2p.Install (n0n1);
    p2p.SetDeviceAttribute ("DataRate", StringValue ("128Kbps"));
    NetDeviceContainer d2d1 = p2p.Install (n2n1);

    Ipv4AddressHelper ipv4;

    ipv4.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i0i1 = ipv4.Assign (d0d1);

    ipv4.SetBase ("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer i2i1 = ipv4.Assign (d2d1);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    Ipv4GlobalRoutingHelper g;
    Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>
            ("dynamic-global-routing.routes", std::ios::out);
    g.PrintRoutingTableAllAt (Seconds (12), routingStream);

    uint16_t port = 9;

    OnOffHelper onoff ("ns3::UdpSocketFactory",
                       Address (InetSocketAddress (i2i1.GetAddress (0), port)));

    onoff.SetConstantRate (DataRate ("256Kbps"));
    onoff.SetAttribute("PacketSize", UintegerValue(256));

    ApplicationContainer apps = onoff.Install (c.Get (0));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

    PacketSinkHelper sink ("ns3::UdpSocketFactory",
                           Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
    apps = sink.Install (c.Get (2));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));

    //FlowMonitor
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> Monitor = flowmon.InstallAll();
    Monitor->SetAttribute ("DelayBinWidth", DoubleValue(0.001));
    Monitor->SetAttribute ("JitterBinWidth",DoubleValue (0.001));

    AsciiTraceHelper ascii;
    p2p.EnableAsciiAll (ascii.CreateFileStream ("queuetest-routing.tr"));
    p2p.EnablePcapAll ("queuetest-routing");

    AnimationInterface anim("queuetest-routing.xml");
    anim.SetConstantPosition(n0n1.Get(0),10.0,10.0);
    anim.SetConstantPosition(n0n1.Get(1),10.0,30.0);
    anim.SetConstantPosition(n2n1.Get(0),10.0,50.0);

    Simulator::Stop (Seconds (11));
    Simulator::Run ();

    Monitor->SerializeToXmlFile ("queuetest-results.xml",true,true);

    Simulator::Destroy ();
    return 0;
}