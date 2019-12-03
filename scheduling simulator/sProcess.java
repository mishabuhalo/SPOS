public class sProcess {
  public int cputime;
  public int ioblocking;
  public int cpudone;
  public int ionext;
  public int numblocked;
  public int ComingTime;
  public int BlockTime;
  public int UnblockTime;

  public sProcess (int cputime, int ioblocking, int cpudone, int ionext, int numblocked, int ComingTime, int BlockTime, int UnblockTime) {
    this.cputime = cputime;
    this.ioblocking = ioblocking;
    this.cpudone = cpudone;
    this.ionext = ionext;
    this.numblocked = numblocked;
    this.ComingTime = ComingTime;
    this.BlockTime = BlockTime;
    this.UnblockTime = UnblockTime;
  } 	
}
