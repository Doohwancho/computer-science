#include <stdio.h>
int main()
{
    int A[100][4]; // Matrix for storing Process Id, Burst
                   // Time, Average Waiting Time & Average
                   // Turn Around Time.
    int i, j, n, total = 0, index, temp;
    float avg_wt, avg_tat;
    printf("Enter number of process: ");
    scanf("%d", &n);
    printf("Enter Burst Time:\n");
    // User Input Burst Time and alloting Process Id.
    for (i = 0; i < n; i++) {
        printf("P%d: ", i + 1);
        scanf("%d", &A[i][1]);
        A[i][0] = i + 1;
    }
    // Sorting process according to their Burst Time.
    for (i = 0; i < n; i++) {
        index = i;
        for (j = i + 1; j < n; j++)
            if (A[j][1] < A[index][1])
                index = j;
        temp = A[i][1];
        A[i][1] = A[index][1];
        A[index][1] = temp;

        temp = A[i][0];
        A[i][0] = A[index][0];
        A[index][0] = temp;
    }
    A[0][2] = 0;
    // Calculation of Waiting Times
    for (i = 1; i < n; i++) {
        A[i][2] = 0;
        for (j = 0; j < i; j++)
            A[i][2] += A[j][1];
        total += A[i][2];
    }
    avg_wt = (float)total / n;
    total = 0;
    printf("P     BT     WT     TAT\n");
    // Calculation of Turn Around Time and printing the
    // data.
    for (i = 0; i < n; i++) {
        A[i][3] = A[i][1] + A[i][2];
        total += A[i][3];
        printf("P%d     %d     %d      %d\n", A[i][0],
               A[i][1], A[i][2], A[i][3]);
    }
    avg_tat = (float)total / n;
    printf("Average Waiting Time= %f", avg_wt);
    printf("\nAverage Turnaround Time= %f", avg_tat);
}



//console.log

//3개 프로세스가 도는데,
//젤 먼저 도착한애 burst time이 100ms = 겁나 느림.
//그래서 제일 뒤로 뺌.
//burst time이 젤 빠른 P2가 첫번째로 처리되고, 그 다음으로 빠른 P3이 두번째, P1이 세번째로 처리됨

//총 걸리는 시간은 서순 어떻게하든 같겠지만,
//average waiting time은 매우 줄어듬.
//무지성 순차처리 대비.

/* Enter number of process: 3 */
/* Enter Burst Time: */
/* P1: 100 */
/* P2: 1 */
/* P3: 2 */
/* P     BT     WT     TAT */
/* P2     1     0      1 */
/* P3     2     1      3 */
/* P1     100     3      103 */
/* Average Waiting Time= 1.333333 */
/* Average Turnaround Time= 35.666668% */
