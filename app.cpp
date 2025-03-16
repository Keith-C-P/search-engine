#include <iostream>
#include <vector>
#include <cstring>
#include <ncurses.h>
#include <fstream>
#include <bits/stdc++.h>
// using namespace std;

WINDOW *create_new_win(int height, int width, int starty, int startx);
WINDOW *create_new_bordered_win(int height, int width, int starty, int startx);

class SearchSort
{
private:
    static int compare(const char *a, const char *b)
    {
        return strcmp(a, b) < 0 ? 1 : 0;
    }

    static void merge(std::vector<const char *> &arr, int left, int mid, int right)
    {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<const char *> L(n1), R(n2);

        for (int i = 0; i < n1; i++)
            L[i] = arr[left + i];
        for (int j = 0; j < n2; j++)
            R[j] = arr[mid + 1 + j];

        int i = 0, j = 0;
        int k = left;

        while (i < n1 && j < n2)
        {
            if (SearchSort::compare(L[i], R[j]))
            {
                arr[k] = L[i];
                i++;
            }
            else
            {
                arr[k] = R[j];
                j++;
            }
            k++;
        }
        while (i < n1)
        {
            arr[k] = L[i];
            i++;
            k++;
        }

        while (j < n2)
        {
            arr[k] = R[j];
            j++;
            k++;
        }
    }

    static void swap(const char **a, const char **b)
    {
        const char *temp = *a;
        *a = *b;
        *b = temp;
    }

    static inline int partition(std::vector<const char *> &arr, long unsigned low, long unsigned high)
    {
        const char *pivot = arr[high];
        long unsigned i = low - 1;

        for (long unsigned j = low; j <= high - 1; j++)
        {
            if (SearchSort::compare(arr[j], pivot))
            {
                i++;
                SearchSort::swap(&arr[i], &arr[j]);
            }
        }
        SearchSort::swap(&arr[i + 1], &arr[high]);
        return i + 1;
    }

public:
    static int containsPrefix(const char *prefix, const char *word)
    {
        return strncmp(word, prefix, strlen(prefix)) == 0;

        // if (strlen(prefix) > strlen(word))
        //     return 0;

        // for (int i = 0; i < strlen(prefix); i++)
        //     if (prefix[i] != word[i])
        //         return 0;

        // return 1;
    }

    static inline void mergeSort(std::vector<const char *> &arr, int left, int right)
    {
        if (left >= right)
            return;

        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }

    static inline void bubbleSort(std::vector<const char *> &arr)
    {
        int n = arr.size();
        for (int i = 0; i < n - 1; i++)
            for (int j = 0; j < n - i - 1; j++)
                if (arr[j] > arr[j + 1])
                    std::swap(arr[j], arr[j + 1]);
    }

    static inline int binarySearch(std::vector<const char *> &arr, const char *search)
    {
        long unsigned low = 0;
        long unsigned high = arr.size() - 1;
        long unsigned mid = low + high / 2;
        while (low <= high)
        {
            mid = low + (high - low) / 2;
            if (arr[mid] == search)
                return mid;
            if (arr[mid] < search)
                low = mid + 1;
            else
                high = mid - 1;
        }

        return -1;
    }

    static inline void quickSort(std::vector<const char *> &arr, long unsigned low, long unsigned high)
    {
        if (low < high)
        {
            int pi = SearchSort::partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }
};

class Search_Bar // hide cursor and implement typing
{
public:
    WINDOW *search_bar;
    std::string search_word;
    int height, width, starty, startx;
    Search_Bar() : height(3), startx(0), starty(0)
    {
        int __rows, cols;
        getmaxyx(stdscr, __rows, cols);
        this->width = cols;
        this->search_bar = create_new_bordered_win(3, cols, 0, 0);
        mvwprintw(search_bar, 0, 1, "Search");
        wrefresh(search_bar);
    }
    Search_Bar(int height, int width, int starty, int startx) : height(height), width(width), starty(starty), startx(startx)
    {
        search_bar = create_new_bordered_win(height, width, starty, startx);
        mvwprintw(search_bar, 0, 1, "Search");
        wrefresh(search_bar);
    }

    int update_search_word(int ch)
    {
        if (ch == KEY_BACKSPACE || ch == 127)
        {
            if (!search_word.empty())
            {
                search_word.pop_back();
                return 1; // a change has occured
            }
        }
        else if (ch >= 32 && ch <= 126)
        {
            search_word.push_back(ch);
            return 1; // a change has occured
        }
        return 0;
    }

    void display_search_word()
    {
        werase(search_bar);
        box(search_bar, 0, 0);
        mvwprintw(search_bar, 0, 1, "Search");
        mvwprintw(search_bar, 1, 1, "%s", search_word.c_str());
        wrefresh(search_bar);
    }
};

class Results
{
public:
    WINDOW *results;
    int height, width, startx, starty, cursorpos = 0, selectedpage = 0;
    long unsigned int startrow;
    std::vector<const char *> pages, filtered_pages;
    Search_Bar *search_bar;
    Results(Search_Bar *search_bar) : width(search_bar->width), startx(0), starty(search_bar->height), startrow(0), search_bar(search_bar)
    {
        int rows, __cols;
        getmaxyx(stdscr, rows, __cols);
        this->height = rows - search_bar->height;
        results = create_new_win(height, width, starty, startx);
        scrollok(this->results, TRUE);
        wrefresh(results);
    }

    Results(int height, int width, int starty, int startx) : height(height), width(width), startx(startx), starty(starty), startrow(0)
    {
        results = create_new_win(height, width, starty, startx);
        scrollok(this->results, TRUE);
        wrefresh(results);
    }

    void load_pages(std::vector<const char *> pages)
    {
        this->pages = pages;
        redraw_results();
        wrefresh(results);
    }

    void move_cursor(int ch)
    {
        int max_cursor_pos = this->pages.size() - 1;
        switch (ch)
        {
        case KEY_UP:
        case KEY_PPAGE:
            update_cursor(cursorpos - 1);
            break;
        case KEY_DOWN:
        case KEY_NPAGE:
            update_cursor(cursorpos + 1);
            break;
        }
        wrefresh(this->results);
    }

    void update_cursor(long unsigned newrow)
    {
        if (search_bar->search_word.empty())
        {
            if (newrow >= pages.size())
                return;
        }
        else
        {
            if (newrow >= filtered_pages.size())
                return;
        }

        if (newrow < startrow)
            startrow--;
        else if (newrow > startrow + height - 1)
            startrow++;
        this->cursorpos = newrow;
        redraw_results();
    }

    void redraw_results()
    {
        werase(this->results);

        if (search_bar->search_word.empty())
        {
            for (long unsigned i = 0; i < pages.size() && i < startrow + height; i++)
                mvwprintw(this->results, i, 0, "  %s", this->pages[startrow + i]);
            mvwprintw(this->results, cursorpos - startrow, 0, "->");
        }
        else
        {
            for (long unsigned i = 0; i < filtered_pages.size() && i < startrow + height; i++)
                mvwprintw(this->results, i, 0, "  %s", this->filtered_pages[startrow + i]);
            mvwprintw(this->results, cursorpos - startrow, 0, "->");
        }
        wrefresh(this->results);
    }

    void filter_results()
    {
        cursorpos = 0; // Reset cursor position when filtering
        startrow = 0;  // Reset start row to ensure proper scrolling
        long long idx = SearchSort::binarySearch(this->pages, search_bar->search_word.c_str());

        filtered_pages.clear();
        for (long long i = idx; i > 0; i++) // append the left elements of the search word
        {
            if (!SearchSort::containsPrefix(search_bar->search_word.c_str(), pages[i]))
                break;
            filtered_pages.push_back(pages[i]);
        }

        for (long unsigned int i = idx + 1; i < pages.size(); i++) // append the right elemts of the search word
        {
            if (SearchSort::containsPrefix(search_bar->search_word.c_str(), pages[i]))
                filtered_pages.push_back(pages[i]);
        }
        SearchSort::mergeSort(filtered_pages, 0, filtered_pages.size() - 1);
        this->redraw_results();
    }
};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage:" << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cout << "Error: Could not open file " << argv[1] << "\n";
        return 1;
    }
    std::vector<const char *> pages;
    std::string page;
    while (getline(file, page))
    {
        pages.push_back((const char *)strdup(page.c_str()));
    }

    file.close();
    // std::vector<const char *> pages = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
    // SearchSort::bubbleSort(pages);
    SearchSort::mergeSort(pages, 0, pages.size() - 1);
    // SearchSort::quickSort(pages, 0, pages.size() - 1);

    initscr();
    noecho();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    ESCDELAY = 25;

    refresh();
    Search_Bar search_bar = Search_Bar();
    Results results = Results(&search_bar);

    results.load_pages(pages);
    int ch;
    while ((ch = getch()) != 27) // 27 is ESC Key
    {
        if (search_bar.update_search_word(ch))
        {
            search_bar.display_search_word();
            results.filter_results();
        }
        results.move_cursor(ch);
    }
    endwin();
    for (const char *p : pages)
        free((void *)p);
    return 0;
}

WINDOW *create_new_bordered_win(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    wrefresh(local_win);

    return local_win;
}

WINDOW *create_new_win(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    wrefresh(local_win);
    return local_win;
}
