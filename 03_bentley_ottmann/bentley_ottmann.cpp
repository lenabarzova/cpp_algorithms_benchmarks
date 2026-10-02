#include <vector>
#include <set>
#include <queue>
#include <cmath>

using namespace std;

struct Point { double x, y; };
struct Segment { Point a, b; };
double X;
const vector<Segment>* S;
const double EPS = 1e-10;

struct SegmentPair {
    int a, b;
    bool operator<(const SegmentPair& other) const {
        if (a != other.a) {
            return a < other.a;
        }
        return b < other.b;
    }
};


struct Boundary {int a, b;};

double yAt(int i) {
    Segment s = (*S)[i];
    return s.a.y +
        (s.b.y - s.a.y) * (X - s.a.x)/(s.b.x - s.a.x);
}


struct SegCmp {
    bool operator()(int a, int b) const {
        if (a == b) {
            return false;
        }
        double ya = yAt(a);
        double yb = yAt(b);
        if (ya != yb) {
            return ya < yb;
        }
        return a < b;
    }
};

struct Event {
    double x, y;
    int type;
    int a, b;
};


struct EventCmp {
    bool operator()(const Event& a, const Event& b) const {
        if (a.x != b.x) {
            return a.x > b.x;
        }
        if (a.y != b.y) {
            return a.y > b.y;
        }
        if (a.type != b.type) {
            return a.type > b.type;
        }
        if (a.a != b.a) {
            return a.a > b.a;
        }
        return a.b > b.b;
    }
};

double cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

bool samePoint(Point a, Point b) {
    return fabs(a.x - b.x) < EPS &&
        fabs(a.y - b.y) < EPS;
}

bool intersectionPoint(const Segment& a, const Segment& b, Point& p) {
    Point r = { a.b.x - a.a.x, a.b.y - a.a.y};
    Point s = {b.b.x - b.a.x, b.b.y - b.a.y};
    Point q = { b.a.x - a.a.x, b.a.y - a.a.y};
    double d = cross(r, s);
    if (fabs(d) < 1e-12) {
        return false;
    }
    double t = cross(q, s) / d;
    double u = cross(q, r) / d;

    if (t <= 0 || t >= 1 || u <= 0 || u >= 1) {
        return false;
    }
    p.x = a.a.x + t * r.x;
    p.y = a.a.y + t * r.y;
    return true;
}

SegmentPair makePair(int a, int b) {
    SegmentPair p;
    if (a < b) {
        p.a = a;
        p.b = b;
    }
    else {
        p.a = b;
        p.b = a;
    }
    return p;
}

bool areAdjacent(set<int, SegCmp>& active, int a, int b) {
    set<int, SegCmp>::iterator ia = active.find(a);
    set<int, SegCmp>::iterator ib = active.find(b);
    if (ia == active.end() || ib == active.end()) {
        return false;
    }
    set<int, SegCmp>::iterator nextA = ia;
    ++nextA;

    if (nextA != active.end() && *nextA == b) {
        return true;
    }

    set<int, SegCmp>::iterator nextB = ib;
    ++nextB;

    if (nextB != active.end() && *nextB == a) {
        return true;
    }
    return false;
}

void addEvent(int a, int b, double currentX, const vector<Segment>& segments,
    set<SegmentPair>& scheduled,
    priority_queue<Event, vector<Event>, EventCmp>& events) {

    if (a >= 0 && b >= 0 && a != b) {
        SegmentPair key = makePair(a, b);

        if (scheduled.count(key) == 0) {
            Point p;

            if (intersectionPoint(segments[a], segments[b], p)) {
                if (p.x > currentX + EPS) {
                    scheduled.insert(key);

                    Event e;
                    e.x = p.x;
                    e.y = p.y;
                    e.type = 1;
                    e.a = key.a;
                    e.b = key.b;

                    events.push(e);
                }
            }
        }
    }
}


void checkNeighbors(int id, double currentX, set<int, SegCmp>& active,
    const vector<Segment>& segments,
    set<SegmentPair>& scheduled,
    priority_queue<Event, vector<Event>, EventCmp>& events) {

    set<int, SegCmp>::iterator it = active.find(id);

    if (it != active.end()) {
        if (it != active.begin()) {
            set<int, SegCmp>::iterator previous = it;
            --previous;

            addEvent(*previous, *it, currentX, segments, scheduled, events);
        }

        set<int, SegCmp>::iterator nextIt = it;
        ++nextIt;

        if (nextIt != active.end()) {
            addEvent(*it, *nextIt, currentX, segments, scheduled, events);
        }
    }
}

long long bentley(const vector<Segment>& input) {
    vector<Segment> segments = input;

    for (int i = 0; i < (int)segments.size(); i++) {
        if (segments[i].a.x > segments[i].b.x) {
            Point temp = segments[i].a;
            segments[i].a = segments[i].b;
            segments[i].b = temp;
        }
    }

    S = &segments;

    priority_queue<Event, vector<Event>, EventCmp> events;

    for (int i = 0; i < (int)segments.size(); i++) {

        if (fabs(segments[i].a.x - segments[i].b.x) < EPS) {
            Event e;

            e.x = segments[i].a.x;
            e.y = segments[i].a.y;
            e.type = 3;
            e.a = i;
            e.b = -1;

            events.push(e);
        }

        else {
            Event start;

            start.x = segments[i].a.x;
            start.y = segments[i].a.y;
            start.type = 0;
            start.a = i;
            start.b = -1;

            events.push(start);


            Event finish;

            finish.x = segments[i].b.x;
            finish.y = segments[i].b.y;
            finish.type = 2;
            finish.a = i;
            finish.b = -1;

            events.push(finish);
        }
    }


    set<int, SegCmp> active;
    set<SegmentPair> scheduled;

    long long ans = 0;


    while (!events.empty()) {

        double currentX = events.top().x;

        vector<Event> group;

        while (!events.empty() &&
            fabs(events.top().x - currentX) < EPS) {
            group.push_back(events.top());
            events.pop();
        }

        X = currentX - 1e-8;

        vector<Event> crossings;
        vector<int> starts;
        vector<int> finishes;
        vector<int> verticals;

        for (int i = 0; i < (int)group.size(); i++) {

            if (group[i].type == 1) {
                crossings.push_back(group[i]);

                SegmentPair key =
                    makePair(group[i].a, group[i].b);

                scheduled.erase(key);
            }

            else if (group[i].type == 0) {
                starts.push_back(group[i].a);
            }

            else if (group[i].type == 2) {
                finishes.push_back(group[i].a);
            }

            else {
                verticals.push_back(group[i].a);
            }
        }

        for (int i = 0; i < (int)finishes.size(); i++) {
            for (int j = 0; j < (int)starts.size(); j++) {
                Point finishPoint = segments[finishes[i]].b;
                Point startPoint = segments[starts[j]].a;
                if (samePoint(finishPoint, startPoint)) {
                    ans++;
                }
            }
        }

        set<int> crossingIds;

        for (int i = 0; i < (int)crossings.size(); i++) {

            int a = crossings[i].a;
            int b = crossings[i].b;

            if (areAdjacent(active, a, b)) {
                ans++;

                crossingIds.insert(a);
                crossingIds.insert(b);
            }
        }

        for (int i = 0; i < (int)verticals.size(); i++) {

            int vertical = verticals[i];

            set<int, SegCmp>::iterator it = active.begin();

            while (it != active.end()) {
                Point p;
                if (intersectionPoint(segments[vertical], segments[*it], p)) {
                    ans++;
                }
                ++it;
            }
        }

        set<int> removeIds;
        set<int>::iterator crossingIt = crossingIds.begin();

        while (crossingIt != crossingIds.end()) {
            removeIds.insert(*crossingIt);
            ++crossingIt;
        }
        for (int i = 0; i < (int)finishes.size(); i++) {
            removeIds.insert(finishes[i]);
        }

        vector<Boundary> boundaries;

        for (int i = 0; i < (int)finishes.size(); i++) {

            set<int, SegCmp>::iterator it =
                active.find(finishes[i]);

            if (it != active.end()) {

                int left = -1;
                int right = -1;

                set<int, SegCmp>::iterator p = it;
                bool foundLeft = false;

                while (p != active.begin() && !foundLeft) {
                    --p;
                    if (removeIds.count(*p) == 0) {
                        left = *p;
                        foundLeft = true;
                    }
                }

                set<int, SegCmp>::iterator n = it;
                ++n;

                bool foundRight = false;

                while (n != active.end() && !foundRight) {
                    if (removeIds.count(*n) == 0) {
                        right = *n;
                        foundRight = true;
                    }
                    else {
                        ++n;
                    }
                }


                if (left >= 0 && right >= 0) {
                    Boundary boundary;

                    boundary.a = left;
                    boundary.b = right;
                    boundaries.push_back(boundary);
                }
            }
        }

        set<int>::iterator removeIt = removeIds.begin();

        while (removeIt != removeIds.end()) {

            set<int, SegCmp>::iterator it =
                active.find(*removeIt);

            if (it != active.end()) {
                active.erase(it);
            }
            ++removeIt;
        }

        X = currentX + 1e-8;

        vector<int> inserted;

        crossingIt = crossingIds.begin();

        while (crossingIt != crossingIds.end()) {
            active.insert(*crossingIt);
            inserted.push_back(*crossingIt);
            ++crossingIt;
        }

        for (int i = 0; i < (int)starts.size(); i++) {
            active.insert(starts[i]);
            inserted.push_back(starts[i]);
        }

        for (int i = 0; i < (int)inserted.size(); i++) {
            checkNeighbors(inserted[i], currentX, active, segments, scheduled, events);
        }

        for (int i = 0; i < (int)boundaries.size(); i++) {
            int a = boundaries[i].a;
            int b = boundaries[i].b;
            if (areAdjacent(active, a, b)) {
                addEvent(a, b, currentX, segments, scheduled, events);
            }
        }
    }

    return ans;
}