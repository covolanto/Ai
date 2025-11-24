

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUEUE 100
#define MAX_LOG 200


typedef enum { FIRE, MEDICAL, POLICE } EventType;
const char* type_names[] = {"Fire", "Medical", "Police"};


typedef struct {
    EventType type;
    int time_reported;
    int proximity;
    int urgency;
    int time_of_day;
    int weather;
    int priority;
} Event;


Event fire_queue[MAX_QUEUE], medical_queue[MAX_QUEUE], police_queue[MAX_QUEUE];
int fire_len = 0, medical_len = 0, police_len = 0;


typedef struct {
    int time;
    EventType dispatcher;
    int priority;
    int proximity;
} LogEntry;

LogEntry event_log[MAX_LOG];
int log_len = 0;


int score_event(Event e) {
    int score = 0;
    if (e.type == FIRE) score += 30;
    if (e.type == MEDICAL) score += 40;
    if (e.type == POLICE) score += 25;

    score += (10 - e.proximity) * 2;
    score += e.urgency * 3;
    if (e.weather == 1) score += 5;
    if (e.time_of_day >= 22 || e.time_of_day <= 6) score += 5;

    return score;
}


void add_event(Event e) {
    e.priority = score_event(e);
    if (e.type == FIRE && fire_len < MAX_QUEUE)
        fire_queue[fire_len++] = e;
    else if (e.type == MEDICAL && medical_len < MAX_QUEUE)
        medical_queue[medical_len++] = e;
    else if (e.type == POLICE && police_len < MAX_QUEUE)
        police_queue[police_len++] = e;
}


Event* get_highest_priority_event(EventType* which_dispatcher) {
    Event* top = NULL;
    int best_score = -1;
    if (fire_len > 0 && fire_queue[0].priority > best_score) {
        top = &fire_queue[0]; *which_dispatcher = FIRE; best_score = top->priority;
    }
    if (medical_len > 0 && medical_queue[0].priority > best_score) {
        top = &medical_queue[0]; *which_dispatcher = MEDICAL; best_score = top->priority;
    }
    if (police_len > 0 && police_queue[0].priority > best_score) {
        top = &police_queue[0]; *which_dispatcher = POLICE; best_score = top->priority;
    }
    return top;
}


void remove_event(EventType type) {
    int i;
    if (type == FIRE && fire_len > 0) {
        for (i = 1; i < fire_len; i++) fire_queue[i - 1] = fire_queue[i];
        fire_len--;
    } else if (type == MEDICAL && medical_len > 0) {
        for (i = 1; i < medical_len; i++) medical_queue[i - 1] = medical_queue[i];
        medical_len--;
    } else if (type == POLICE && police_len > 0) {
        for (i = 1; i < police_len; i++) police_queue[i - 1] = police_queue[i];
        police_len--;
    }
}

void run_simulation(int steps) {
    int t;
    for (t = 0; t < steps; t++) {
        EventType dispatcher;
        Event* e = get_highest_priority_event(&dispatcher);
        if (e) {
            printf("[Time %d] %s dispatched (priority %d, proximity %d)\n", t, type_names[dispatcher], e->priority, e->proximity);
            event_log[log_len++] = (LogEntry){t, dispatcher, e->priority, e->proximity};
            remove_event(dispatcher);
        } else {
            printf("[Time %d] No event to dispatch\n", t);
        }
    }
}

int main() {

    Event e1 = {MEDICAL, 0, 3, 5, 10, 1};
    Event e2 = {FIRE, 0, 6, 4, 23, 0};
    Event e3 = {POLICE, 0, 5, 2, 14, 0};
    Event e4 = {MEDICAL, 0, 2, 4, 8, 1};
    Event e5 = {FIRE, 0, 8, 1, 12, 0};

    add_event(e1);
    add_event(e2);
    add_event(e3);
    add_event(e4);
    add_event(e5);

    run_simulation(10);

    return 0;
}

