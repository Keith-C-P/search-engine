import wikipediaapi as wiki
def load(x: float):
    x = int(x * 10)
    if x % 10 > load.percent_done:
        return "-"
    return ""

def get_links(wiki_connector, file, page_name, x):
    if x <= 0:
        return
    page_py: wiki.WikipediaPage = wiki_connector.page(page_name)
    links : wiki.PagesDict = page_py.links
    for title in links.keys():
        if title not in get_links.title_set:
            get_links.title_set.add(title)
            file.write(f"{title}\n")
            get_links(wiki_connector, file, title, x-1)

if __name__ == "__main__":
    DEPTH = 5 #edit this to change the depth
    SOURCE_SITE = "Python_(programming_language)" #edit this to change the source page (make sure it exists) 
    wiki_connector = wiki.Wikipedia(user_agent='WikiBrowser', language='en')

    get_links.title_set = set()
    with open("data.txt", "w") as f:
        get_links(wiki_connector, f, SOURCE_SITE, DEPTH)
