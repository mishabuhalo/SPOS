// Run() is called from Scheduling.main() and is where
// the scheduling algorithm written by the user resides.
// User modification should occur within the Run() function.

import java.util.Vector;
import java.io.*;

public class SchedulingAlgorithm {

  private static  int FindShortestTimeRemaningProcess(Vector processVector, int comptime)
  {
    int size = processVector.size();
    int minimum = Integer.MAX_VALUE;
    int processIndex = -1;
    sProcess process;
    for(int i = 0; i < size; i++)
    {
      process = (sProcess) processVector.elementAt(i);
      if(process.ComingTime <= comptime && process.UnblockTime ==-1 && process.cputime > process.cpudone && (process.cputime-process.cpudone < minimum))
      {
        minimum = process.cputime-process.cpudone;
        processIndex = i;
      }
    }
    return processIndex;
  }
  public static Results Run(int runtime, Vector processVector, Results result) {
    int comptime = 0;
    int currentProcess = FindShortestTimeRemaningProcess(processVector, comptime);
    int size = processVector.size();
    int completed = 0;
    String resultsFile = "Summary-Processes";

    result.schedulingType = "Batch (preemptive)";
    result.schedulingName = "Shortest remaining time first\n";

    boolean Flag = false;
    boolean IsProcessRunning = true;
    try {
      PrintStream out = new PrintStream(new FileOutputStream(resultsFile));
      sProcess process = (sProcess) processVector.elementAt(currentProcess);
      out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.ComingTime + ")");

      while (comptime < runtime) {
        for (int i = 0; i < size - 1; i++) {
          sProcess TempProcess = (sProcess) processVector.elementAt(i);
          if (TempProcess.ComingTime == comptime)
            Flag = true;
          if (TempProcess.UnblockTime == comptime) {
            TempProcess.UnblockTime = -1;
            Flag = true;
          }
        }

        if (!IsProcessRunning && Flag) {
          currentProcess = FindShortestTimeRemaningProcess(processVector, comptime);
          if (currentProcess == -1) {
            comptime++;
            continue;
          }
          process = (sProcess) processVector.elementAt(currentProcess);
          out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.ComingTime + ")");
          IsProcessRunning = true;
          Flag = false;
        }

        if (IsProcessRunning) {

          if (process.cpudone == process.cputime) {
            completed++;
            out.println("Process: " + currentProcess + " completed... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.ComingTime + ")");
            if (completed == size) {
              result.compuTime = comptime;
              out.close();
              return result;
            }

            currentProcess = FindShortestTimeRemaningProcess(processVector, comptime);
            Flag = false;
            if (currentProcess == -1) {
              comptime++;
              IsProcessRunning = false;
              continue;
            }

            process = (sProcess) processVector.elementAt(currentProcess);
            out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.ComingTime + ")");
          }
          if (process.ioblocking == process.ionext) {
            out.println("Process: " + currentProcess + " I/O blocked... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.ComingTime + ")");
            process.numblocked++;
            process.ionext = 0;
            process.UnblockTime = comptime + process.BlockTime;
            currentProcess = FindShortestTimeRemaningProcess(processVector, comptime);
            Flag = false;
            if (currentProcess == -1) {
              comptime++;
              IsProcessRunning = false;
              continue;
            }

            process = (sProcess) processVector.elementAt(currentProcess);
            out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.ComingTime + ")");
          }


          if (Flag) {
            int TempProcess = FindShortestTimeRemaningProcess(processVector, comptime);
            if (TempProcess != currentProcess) {
              out.println("Process: " + currentProcess + " interrupted... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.ComingTime + ")");

              currentProcess = TempProcess;
              process = (sProcess) processVector.elementAt(currentProcess);
              out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.ComingTime + ")");
              Flag = false;
            }
          }
          process.cpudone++;
          if (process.ioblocking > 0) {
            process.ionext++;
          }
        }
          comptime++;

      }
      out.close();
    } catch (IOException e) { /* Handle exceptions */ }
    result.compuTime = comptime;
    return result;
  }
}
