#include <Riostream.h>

#include "../custom_classes/event/event.hpp"

void testEvent()
{
    Detector detector1 = {3., 0.08, 27, false};

    Event * event = new Event();
    Vertex ver = event->partGeneration();
    event->partTransport(detector1);

    cout << "ver = (" << ver.getX() << ", " << ver.getY() << ", " << ver.getZ() << endl;

    cout << "Hello world!" << endl;
}