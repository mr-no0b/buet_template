struct Hungerian {
    vector<vector<int>> cost;
    vector<int> worker, job, parentjob, matchjob, assign;
    int n;
    Hungerian(int n) : n(n) {
        cost.resize(n, vector<int>(n));
        worker.resize(n + 1);
        job.resize(n + 1);
        parentjob.resize(n + 1);
        matchjob.resize(n + 1);
    }
    int solve() {
        for(int w = 1; w <= n; w++) {
            matchjob[0] = w;
            vector<int> minslack(n + 1, INT_MAX);
            vector<bool> used(n + 1, 0);
            int job0 = 0;
            do {
                used[job0] = true;
                int worker0 = matchjob[job0], delta = INT_MAX, job1 = 0;
                for(int j = 1; j <= n; j++) {
                    if(!used[j]) {
                        int curcost = cost[worker0 - 1][j - 1] - worker[worker0] - job[j];
                        if(curcost < minslack[j]) {
                            minslack[j] = curcost;
                            parentjob[j] = job0;
                        }
                        if(minslack[j] < delta) {
                            delta = minslack[j];
                            job1 = j;
                        }
                    }
                }
                for(int j = 0; j <= n; j++) {
                    if(used[j]) {
                    worker[matchjob[j]] += delta;
                    job[j] -= delta;
                    }
                    else {
                        minslack[j] -= delta;
                    }
                } 
                job0 = job1;
            } while(matchjob[job0] != 0);

            do {
                int job1 = parentjob[job0];
                matchjob[job0] = matchjob[job1];
                job0 = job1;
            } while(job0);
        }

        assign.assign(n + 1, 0);
        for(int j = 1; j <= n; j++) {
            assign[matchjob[j]] = j;
        }

        return -job[0];
    }
};
