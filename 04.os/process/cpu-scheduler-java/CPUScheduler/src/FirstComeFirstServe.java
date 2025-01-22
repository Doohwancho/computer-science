
import java.util.Collections;
import java.util.List;

public class FirstComeFirstServe extends CPUScheduler
{
    //TODO - FCFS algorithm revealed!
    @Override
    public void process()
    {
        //step1) arrival time(AT)로 ASC 정렬
        Collections.sort(this.getRows(), (Object o1, Object o2) -> {
            if (((Row) o1).getArrivalTime() == ((Row) o2).getArrivalTime())
            {
                return 0;
            }
            else if (((Row) o1).getArrivalTime() < ((Row) o2).getArrivalTime())
            {
                return -1;
            }
            else
            {
                return 1;
            }
        });
        
        List<Event> timeline = this.getTimeline();
        
        //step2) calculate finish times (accumulate BTs' of processes!)
        for (Row row : this.getRows())
        {
            if (timeline.isEmpty()) //첫 process면?
            {
                timeline.add(new Event(row.getProcessName(), row.getArrivalTime(), row.getArrivalTime() + row.getBurstTime())); //finish time = fisr process's AT+BT
            }
            else //그 다음 process 부터는?
            {
                Event event = timeline.get(timeline.size() - 1); //get last timeline(Event)
                timeline.add(new Event(row.getProcessName(), event.getFinishTime(), event.getFinishTime() + row.getBurstTime())); //add last timeline's finish time + this process's BT
            }
        }
        
        //step3)
        for (Row row : this.getRows())
        {
            //calculate WT
            row.setWaitingTime(this.getEvent(row).getStartTime() - row.getArrivalTime()); //start time - AT(arrival time) = WT (wait time)
            
            //calculate TAT
            row.setTurnaroundTime(row.getWaitingTime() + row.getBurstTime()); //WT (wait time) + BT (burst time) = TAT (turn around time, a.k.a total time spent)
        }
    }
}
