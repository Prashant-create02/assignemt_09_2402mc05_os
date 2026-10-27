#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_FRAMES 16
#define MAX_REFS   64

static void
print_frame_state(int *frames, int nframes, int ref, int is_hit)
{
  printf(1, "Ref %d -> [ ", ref);
  for(int i = 0; i < nframes; i++){
    if(frames[i] == -1)
      printf(1, "- ");
    else
      printf(1, "%d ", frames[i]);
  }
  printf(1, "] : %s\n", is_hit ? "HIT" : "FAULT");
}

static int
simulate_fifo(int *refs, int nrefs, int nframes, int verbose)
{
  int frames[MAX_FRAMES];
  for(int i = 0; i < nframes; i++)
    frames[i] = -1;

  int fifo_ptr = 0;
  int faults = 0;

  if(verbose)
    printf(1, "\n--- FIFO Page Replacement (%d Frames) ---\n", nframes);

  for(int i = 0; i < nrefs; i++){
    int page = refs[i];
    int hit = 0;

    for(int j = 0; j < nframes; j++){
      if(frames[j] == page){
        hit = 1;
        break;
      }
    }

    if(!hit){
      frames[fifo_ptr] = page;
      fifo_ptr = (fifo_ptr + 1) % nframes;
      faults++;
    }

    if(verbose)
      print_frame_state(frames, nframes, page, hit);
  }

  if(verbose)
    printf(1, "FIFO Total Page Faults: %d | Hits: %d\n", faults, nrefs - faults);

  return faults;
}

static int
simulate_lru(int *refs, int nrefs, int nframes, int verbose)
{
  int frames[MAX_FRAMES];
  int last_used[MAX_FRAMES];

  for(int i = 0; i < nframes; i++){
    frames[i] = -1;
    last_used[i] = -1;
  }

  int faults = 0;

  if(verbose)
    printf(1, "\n--- LRU Page Replacement (%d Frames) ---\n", nframes);

  for(int i = 0; i < nrefs; i++){
    int page = refs[i];
    int hit_idx = -1;

    for(int j = 0; j < nframes; j++){
      if(frames[j] == page){
        hit_idx = j;
        break;
      }
    }

    if(hit_idx != -1){
      last_used[hit_idx] = i;
    } else {
      int replace_idx = -1;
      for(int j = 0; j < nframes; j++){
        if(frames[j] == -1){
          replace_idx = j;
          break;
        }
      }
      if(replace_idx == -1){
        int min_time = last_used[0];
        replace_idx = 0;
        for(int j = 1; j < nframes; j++){
          if(last_used[j] < min_time){
            min_time = last_used[j];
            replace_idx = j;
          }
        }
      }
      frames[replace_idx] = page;
      last_used[replace_idx] = i;
      faults++;
    }

    if(verbose)
      print_frame_state(frames, nframes, page, (hit_idx != -1));
  }

  if(verbose)
    printf(1, "LRU Total Page Faults: %d | Hits: %d\n", faults, nrefs - faults);

  return faults;
}

static void
print_ratio(int hits, int total)
{
  int pct_whole = (hits * 100) / total;
  int pct_frac  = ((hits * 10000) / total) % 100;
  if(pct_frac < 10)
    printf(1, "%d.0%d%%", pct_whole, pct_frac);
  else
    printf(1, "%d.%d%%", pct_whole, pct_frac);
}

int
main(int argc, char *argv[])
{
  int default_refs[20] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
  int refs[MAX_REFS];
  int nrefs = 20;
  int nframes = 3;

  // Allow custom CLI arguments: pagerepl <frames> <ref1> <ref2> ...
  if(argc > 2){
    nframes = atoi(argv[1]);
    nrefs = argc - 2;
    if(nrefs > MAX_REFS)
      nrefs = MAX_REFS;
    for(int i = 0; i < nrefs; i++)
      refs[i] = atoi(argv[i + 2]);

    simulate_fifo(refs, nrefs, nframes, 1);
    simulate_lru(refs, nrefs, nframes, 1);
    exit();
  }

  // Default execution for Tasks (b) and (c)
  for(int i = 0; i < 20; i++)
    refs[i] = default_refs[i];

  printf(1, "Reference String: 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1\n");

  // Task (b): Detailed step-by-step trace for 3 frames
  simulate_fifo(refs, nrefs, 3, 1);
  simulate_lru(refs, nrefs, 3, 1);

  // Task (c): Summary table for 3, 4, and 5 frames
  printf(1, "\n=== Comparison Table (3, 4, and 5 Frames) ===\n");
  printf(1, "Frames | FIFO Faults | FIFO Hit Ratio | LRU Faults | LRU Hit Ratio\n");
  printf(1, "------------------------------------------------------------------\n");

  for(int f = 3; f <= 5; f++){
    int fifo_faults = simulate_fifo(refs, nrefs, f, 0);
    int lru_faults  = simulate_lru(refs, nrefs, f, 0);
    int fifo_hits   = nrefs - fifo_faults;
    int lru_hits    = nrefs - lru_faults;

    printf(1, "  %d    |     %d      |     ", f, fifo_faults);
    print_ratio(fifo_hits, nrefs);
    printf(1, "     |     %d     |     ", lru_faults);
    print_ratio(lru_hits, nrefs);
    printf(1, "\n");
  }

  exit();
}
