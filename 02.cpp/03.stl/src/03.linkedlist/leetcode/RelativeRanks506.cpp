
//solution1

//Runtime: 4 ms, faster than 99.92 % of C++ online submissions for Relative Ranks.
//Memory Usage : 11.3 MB, less than 29.90 % of C++ online submissions for Relative Ranks.

//�� ���Ĺ���� merge sort�� �ᵵ ������Ű�����


class Solution {
public:

    static bool compare(const pair<int, int>& a, const pair<int, int>& b) {
        //If the first number is same
        if (a.first == b.first)
            return a.second > b.second; //The second number in Descending order
        return a.first > b.first; //The first number of bigger numbers to move forward -> Descending order
    }

    vector<string> findRelativeRanks(vector<int>& score) {
        vector<pair<int, int>> v;

        int n = score.size();

        for (int i = 0; i < n; i++) {
            v.push_back(make_pair(score.at(i), i));
        }

        //Descending order
        sort(v.begin(), v.end(), compare);

        int tmp[n];
        for (int i = 0; i < n; i++) {
            tmp[v.at(i).second] = i + 1;
        }

        vector<string> rst;
        for (int i = 0; i < n; i++) {
            if (tmp[i] < 4) {
                switch (tmp[i]) {
                case 1:
                    rst.push_back("Gold Medal");
                    break;
                case 2:
                    rst.push_back("Silver Medal");
                    break;
                case 3:
                    rst.push_back("Bronze Medal");
                    break;
                }
            }
            else {
                rst.push_back(to_string(tmp[i]));
            }
        }

        return rst;
    }
};



//trial02

//� �� �ȵ���

//�޸� �ּ� ������µ� ��....

class Solution {

public:
    vector<pair<int, int>> mergeSort(vector<pair<int, int>>& v, int start, int end) {
        if (start >= end) {
            return v;
        }

        vector<pair<int, int>> left = mergeSort(v, start, end / 2);
        vector<pair<int, int>> right = mergeSort(v, end / 2 + 1, end);

        if (left.at(0).first < right.at(0).first) {
            for (int i = start; i < end; i++) {
                right.push_back(left.at(i));
            }
            return right;
        }
        else {
            for (int i = start; i < end; i++) {
                left.push_back(right.at(i));
            }
            return left;
        }
    }

    vector<string> findRelativeRanks(vector<int>& score) {
        vector<pair<int, int>> v;

        int n = score.size();

        for (int i = 0; i < n; i++) {
            v.push_back(make_pair(score.at(i), i));
        }

        //Descending order
        // sort(v.begin(), v.end(), compare);
        mergeSort(v, 0, v.size() - 1);

        int tmp[n];
        for (int i = 0; i < n; i++) {
            tmp[v.at(i).second] = i + 1;
        }

        vector<string> rst;
        for (int i = 0; i < n; i++) {
            if (tmp[i] < 4) {
                switch (tmp[i]) {
                case 1:
                    rst.push_back("Gold Medal");
                    break;
                case 2:
                    rst.push_back("Silver Medal");
                    break;
                case 3:
                    rst.push_back("Bronze Medal");
                    break;
                }
            }
            else {
                rst.push_back(to_string(tmp[i]));
            }
        }

        return rst;
    }
};


//trial03

//��...

class Solution {

public:
    void merge(vector<pair<int, int>>& v, int l, int r) {
        int m = l + (r - l) / 2;

        // i�� ����, j�� ������, k�� v�� ä�� index
        int i, j, k;

        int left_side_size = m - l + 1; // left ~ mid
        int right_side_size = r - m; // mid+1 ~ right

        // ���ʰ� �������� ������ �迭 1���� ����
        pair* left_side = (pair*)malloc(left_side_size * sizeof(pair));
        pair* right_side = (pair*)malloc(right_side_size * sizeof(pair));

        // �迭 ä���
        for (int p = 0; p < left_side_size; p++) {
            left_side[p] = v[l + p];
        }
        for (int q = 0; q < right_side_size; q++) {
            right_side[q] = v[m + 1 + q];
        }

        // Merge
        // ���� �� ������ �ϳ��� ���ϸ鼭 v�� �߰�
        i = j = 0;
        k = l;
        while (i < left_side_size && j < right_side_size) {
            if (left_side[i] > right_side[j]) {
                v[k] = left_side[i];
                i++;
            }
            else {
                v[k] = right_side[j];
                j++;
            }
            k++;
        }

        // �������� �����ִٸ�
        while (j < right_side_size) {
            v[k] = right_side[j];
            j++;
            k++;
        }

        // ������ �����ִٸ�
        while (i < left_side_size) {
            v[k] = left_side[i];
            i++;
            k++;
        }
        free(left_side); free(right_side);
    }

    void mergeSort(vector<pair<int, int>>& v, int start, int end) {
        if (start >= end) {
            return;
        }

        mergeSort(v, start, end / 2);
        mergeSort(v, end / 2 + 1, end);

        merge(v, start, end);
    }


    vector<string> findRelativeRanks(vector<int>& score) {
        vector<pair<int, int>> v;

        int n = score.size();

        for (int i = 0; i < n; i++) {
            v.push_back(make_pair(score.at(i), i));
        }

        //Descending order
        mergeSort(v, 0, v.size() - 1);

        int tmp[n];
        for (int i = 0; i < n; i++) {
            tmp[v.at(i).second] = i + 1;
        }

        vector<string> rst;
        for (int i = 0; i < n; i++) {
            if (tmp[i] < 4) {
                switch (tmp[i]) {
                case 1:
                    rst.push_back("Gold Medal");
                    break;
                case 2:
                    rst.push_back("Silver Medal");
                    break;
                case 3:
                    rst.push_back("Bronze Medal");
                    break;
                }
            }
            else {
                rst.push_back(to_string(tmp[i]));
            }
        }

        return rst;
    }
};


//trial04


class Solution {

public:
    void merge(vector<pair<int, int>>& v, int l, int r) {
        int m = l + (r - l) / 2;

        // i�� ����, j�� ������, k�� a�� ä�� index
        int i, j, k;

        int left_side_size = m - l + 1; // left ~ mid
        int right_side_size = r - m; // mid+1 ~ right

        // ���ʰ� �������� ������ �迭 1���� ����
        pair<int, int> left_side[left_side_size];
        pair<int, int> right_side[right_side_size];

        // �迭 ä���
        for (auto p = 0; p < left_side_size; p++) {
            left_side[p] = v[l + p];
        }
        for (auto q = 0; q < right_side_size; q++) {
            right_side[q] = v[m + 1 + q];
        }

        // Merge
        // ���� �� ������ �ϳ��� ���ϸ鼭 a�� �߰�
        i = j = 0;
        k = l;
        while (i < left_side_size && j < right_side_size) {
            if (left_side[i] > right_side[j]) {
                v[k] = left_side[i];
                i++;
            }
            else {
                v[k] = right_side[j];
                j++;
            }
            k++;
        }

        // �������� �����ִٸ�
        while (j < right_side_size) {
            v[k] = right_side[j];
            j++;
            k++;
        }

        // ������ �����ִٸ�
        while (i < left_side_size) {
            v[k] = left_side[i];
            i++;
            k++;
        }
    }

    void mergeSort(vector<pair<int, int>>& v, int start, int end) {
        if (start >= end) {
            return;
        }

        mergeSort(v, start, end / 2);
        mergeSort(v, end / 2 + 1, end);

        merge(v, start, end);
    }
    vector<string> findRelativeRanks(vector<int>& score) {
        vector<pair<int, int>> v;

        int n = score.size();

        for (int i = 0; i < n; i++) {
            v.push_back(make_pair(score.at(i), i));
        }

        //Descending order
        mergeSort(v, 0, v.size() - 1);

        int tmp[n];
        for (int i = 0; i < n; i++) {
            tmp[v.at(i).second] = i + 1;
        }

        vector<string> rst;
        for (int i = 0; i < n; i++) {
            if (tmp[i] < 4) {
                switch (tmp[i]) {
                case 1:
                    rst.push_back("Gold Medal");
                    break;
                case 2:
                    rst.push_back("Silver Medal");
                    break;
                case 3:
                    rst.push_back("Bronze Medal");
                    break;
                }
            }
            else {
                rst.push_back(to_string(tmp[i]));
            }
        }

        return rst;
    }
};


//solution 5

//Runtime: 12 ms, faster than 82.86 % of C++ online submissions for Relative Ranks.
//Memory Usage : 11.5 MB, less than 29.19 % of C++ online submissions for Relative Ranks.

//��... ����

//O(N Log N)


class Solution {


public:
    void merge(vector<pair<int, int>>& v, int l, int r, int m) {
        // i�� ����, j�� ������, k�� a�� ä�� index
        int i, j, k;

        int left_side_size = m - l + 1; // left ~ mid
        int right_side_size = r - m; // mid+1 ~ right

        // ���ʰ� �������� ������ �迭 1���� ����
        pair<int, int> left_side[left_side_size];
        pair<int, int> right_side[right_side_size];

        // �迭 ä���
        for (int p = 0; p < left_side_size; p++) {
            left_side[p] = v[l + p];
        }
        // cnt = 0;
        for (int q = 0; q < right_side_size; q++) {
            right_side[q] = v[m + 1 + q];
        }

        //��ġ��
        i = j = 0;
        k = l;
        while (i < left_side_size && j < right_side_size) {
            if (left_side[i] > right_side[j]) {
                v[k] = left_side[i];
                i++;
            }
            else {
                v[k] = right_side[j];
                j++;
            }
            k++;
        }

        // �������� �����ִٸ� ä����
        while (j < right_side_size) {
            v[k] = right_side[j];
            j++;
            k++;
        }

        // ������ �����ִٸ� ä����
        while (i < left_side_size) {
            v[k] = left_side[i];
            i++;
            k++;
        }
    }

    void mergeSort(vector<pair<int, int>>& v, int l, int r) {
        if (l < r) {
            int m = l + (r - l) / 2;

            mergeSort(v, l, m);
            mergeSort(v, m + 1, r);

            merge(v, l, r, m);
        }

    }
    vector<string> findRelativeRanks(vector<int>& score) {
        vector<pair<int, int>> v;

        int n = score.size();

        for (int i = 0; i < n; i++) {
            v.push_back(make_pair(score.at(i), i));
        }

        //Descending order
        mergeSort(v, 0, v.size() - 1);

        int tmp[n];
        for (int i = 0; i < n; i++) {
            tmp[v.at(i).second] = i + 1;
        }

        vector<string> rst;
        for (int i = 0; i < n; i++) {
            if (tmp[i] < 4) {
                switch (tmp[i]) {
                case 1:
                    rst.push_back("Gold Medal");
                    break;
                case 2:
                    rst.push_back("Silver Medal");
                    break;
                case 3:
                    rst.push_back("Bronze Medal");
                    break;
                }
            }
            else {
                rst.push_back(to_string(tmp[i]));
            }
        }

        return rst;
    }
};