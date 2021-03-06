#ifndef __GEOMTK_TimeManager__
#define __GEOMTK_TimeManager__

#include "geomtk_commons.h"

namespace geomtk {

typedef variant<time_duration, days, months, years> duration;

static time_duration
seconds(double x) {
    return time_duration(0, 0, 0, time_duration::ticks_per_second()*x);
} // seconds

static string
ptime_to_string(const ptime &x) {
    stringstream ss;
    ss.imbue(std::locale(cout.getloc(), new time_facet("%Y-%m-%d %H:%M:%s")));
    ss << x;
    return ss.str();
} // ptime_to_string

static duration
durationFromString(const string &str) {
    duration res;
    mark_tag tagSize(1), tagUnit(2);
    sregex reDuration = (tagSize= +_d) >> ' ' >> (tagUnit= +_w);
    smatch what;
    if (regex_match(str, what, reDuration)) {
        double tmp = atoi(what[tagSize].str().c_str());
        if (what[tagUnit] == "hour" ||
            what[tagUnit] == "hours"){
            res = seconds(tmp*3600);
        } else if (what[tagUnit] == "minute" ||
                   what[tagUnit] == "minutes") {
            res = seconds(tmp*60);
        } else if (what[tagUnit] == "second" ||
                   what[tagUnit] == "seconds") {
            res = seconds(tmp);
        } else if (what[tagUnit] == "day" ||
                   what[tagUnit] == "days") {
            res = days(tmp);
        } else if (what[tagUnit] == "month" ||
                   what[tagUnit] == "months") {
            res = months(tmp);
        } else if (what[tagUnit] == "year" ||
                   what[tagUnit] == "years") {
            res = years(tmp);
        } else if (what[tagUnit] == "step" ||
                   what[tagUnit] == "steps") {
            res = time_duration(0, 0, -tmp);
        } else {
            REPORT_ERROR("Invalid step unit \"" << what[tagUnit] << "\"!");
        }
    } else {
        res = duration_from_string(str);
    }
    return res;
} // timeDurationFromString

struct Alarm {
    duration freq;
    ptime lastTime;
    int lastStep;
}; // Alarm

class TimeManager {
protected:
    ptime _startTime, _currTime, _endTime;
    duration _stepSize;
    int _numStep;
    vector<Alarm> alarms;
    bool _isInited;
public:
    TimeManager();
    ~TimeManager();

    template <class DurationType>
    void
    init(const ptime &startTime, const ptime &endTime,
         const DurationType &stepSize);

    void
    init(const ptime &startTime, const ptime &endTime,
         double stepSizeInSeconds);

    void
    init(const string &startTime, const string &endTime,
         double stepSizeInSeconds);

    void
    init(const string &startTime, const string &endTime,
         const string &stepSize);

    bool
    isInited() const {
        return _isInited;
    }

    void
    reset();

    void
    reset(int numStep, const ptime &time);

    template <class FreqType>
    int
    addAlarm(const FreqType &freq);

    bool
    checkAlarm(uword i);

    const Alarm&
    alarm(uword i) const {
        return alarms[i];
    }

    void
    advance(bool mute = false);

    bool
    isFinished() const {
        return _currTime > _endTime;
    }
    
    const ptime&
    startTime() const {
        return _startTime;
    }

    const ptime&
    currTime() const {
        return _currTime;
    }

    const ptime&
    endTime() const {
        return _endTime;
    }

    double
    stepSizeInSeconds() const;

    const duration&
    stepSize() const {
        return _stepSize;
    }

    int
    numStep() const {
        return _numStep;
    }

    int
    totalNumStep() const;

    double
    seconds() const {
        return static_cast<double>((_currTime-_startTime).ticks())/time_duration::ticks_per_second();
    }

    double
    minutes() const {
        return seconds()/60;
    }

    double
    hours() const {
        return minutes()/60;
    }

    double
    days() const {
        return hours()/24;
    }
}; // TimeManager

template <class _DurationType>
void TimeManager::
init(const ptime &startTime, const ptime &endTime,
     const _DurationType &stepSize) {
    if (startTime > endTime) {
        REPORT_ERROR("Start time is less than end time!");
    }
    _startTime = startTime;
    _currTime = startTime;
    _endTime = endTime;
    _stepSize = stepSize;
    _isInited = true;
} // init

template <class FreqType>
int TimeManager::
addAlarm(const FreqType &freq) {
    for (uword i = 0; i < alarms.size(); ++i) {
        if (alarms[i].freq.type() == typeid(FreqType) &&
            boost::get<FreqType>(alarms[i].freq) == freq) {
            return i;
        }
    }
    Alarm alarm;
    alarm.freq = freq;
    assert(_currTime != boost::date_time::not_a_date_time);
    alarm.lastTime = _currTime;
    alarm.lastStep = _numStep;
    alarms.push_back(alarm);
    return alarms.size()-1;
} // addAlarm

} // geomtk

#endif // __GEOMTK_TimeManager__
