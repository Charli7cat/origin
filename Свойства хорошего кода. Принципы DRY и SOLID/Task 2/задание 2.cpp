#include <fstream>
#include <string>
#include <memory>
#include <vector>


class TextPrintable {
public:
    virtual ~TextPrintable() = default;
    virtual std::string toText() const = 0;
};

class HTMLPrintable {
public:
    virtual ~HTMLPrintable() = default;
    virtual std::string toHTML() const = 0;
};

class JSONPrintable {
public:
    virtual ~JSONPrintable() = default;
    virtual std::string toJSON() const = 0;
};


class TextData : public TextPrintable {
public:
    explicit TextData(std::string data) : data_(std::move(data)) {}

    std::string toText() const override {
        return data_;
    }

private:
    std::string data_;
};

class HTMLData : public HTMLPrintable {
public:
    explicit HTMLData(std::string data) : data_(std::move(data)) {}

    std::string toHTML() const override {
        return "<html>" + data_ + "</html>";
    }

private:
    std::string data_;
};

class JSONData : public JSONPrintable {
public:
    explicit JSONData(std::string data) : data_(std::move(data)) {}

    std::string toJSON() const override {
        return "{ \"data\": \"" + data_ + "\" }";
    }

private:
    std::string data_;
};


void saveTo(std::ofstream& file, const TextPrintable& printable) {
    file << printable.toText();
}

void saveTo(std::ofstream& file, const HTMLPrintable& printable) {
    file << printable.toHTML();
}

void saveTo(std::ofstream& file, const JSONPrintable& printable) {
    file << printable.toJSON();
}

void saveToAsText(std::ofstream& file, const TextPrintable& printable) {
    saveTo(file, printable);
}

void saveToAsHTML(std::ofstream& file, const HTMLPrintable& printable) {
    saveTo(file, printable);
}

void saveToAsJSON(std::ofstream& file, const JSONPrintable& printable) {
    saveTo(file, printable);
}


enum class DataFormat {
    Text,
    HTML,
    JSON
};

class DataFactory {
public:
    static std::unique_ptr<TextPrintable> createTextData(const std::string& data) {
        return std::make_unique<TextData>(data);
    }

    static std::unique_ptr<HTMLPrintable> createHTMLData(const std::string& data) {
        return std::make_unique<HTMLData>(data);
    }

    static std::unique_ptr<JSONPrintable> createJSONData(const std::string& data) {
        return std::make_unique<JSONData>(data);
    }

    static std::unique_ptr<void> createData(const std::string& data, DataFormat format) {
        switch (format) {
        case DataFormat::Text:
            return std::make_unique<TextData>(data);
        case DataFormat::HTML:
            return std::make_unique<HTMLData>(data);
        case DataFormat::JSON:
            return std::make_unique<JSONData>(data);
        }
        return nullptr;
    }
};


void example1() {
    std::ofstream file("output.txt");

    TextData text("Hello Text");
    HTMLData html("Hello HTML");
    JSONData json("Hello JSON");

    saveToAsText(file, text);
    saveToAsHTML(file, html);
    saveToAsJSON(file, json);

    saveTo(file, text);
    saveTo(file, html);
    saveTo(file, json);
}

void example2() {
    std::ofstream file("output.txt");

    auto text = DataFactory::createTextData("Hello");
    auto html = DataFactory::createHTMLData("Hello");
    auto json = DataFactory::createJSONData("Hello");

    saveTo(file, *text);
    saveTo(file, *html);
    saveTo(file, *json);
}


class CompositeData : public TextPrintable, public HTMLPrintable, public JSONPrintable {
public:
    explicit CompositeData(std::string data) : data_(std::move(data)) {}

    std::string toText() const override {
        return data_;
    }

    std::string toHTML() const override {
        return "<html>" + data_ + "</html>";
    }

    std::string toJSON() const override {
        return "{ \"data\": \"" + data_ + "\" }";
    }

private:
    std::string data_;
};

void example3() {
    std::ofstream file("output.txt");

    CompositeData data("Hello World");

    saveTo(file, static_cast<const TextPrintable&>(data));
    saveTo(file, static_cast<const HTMLPrintable&>(data));
    saveTo(file, static_cast<const JSONPrintable&>(data));
}


class XMLPrintable {
public:
    virtual ~XMLPrintable() = default;
    virtual std::string toXML() const = 0;
};

class XMLData : public XMLPrintable {
public:
    explicit XMLData(std::string data) : data_(std::move(data)) {}

    std::string toXML() const override {
        return "<data>" + data_ + "</data>";
    }

private:
    std::string data_;
};

void saveTo(std::ofstream& file, const XMLPrintable& printable) {
    file << printable.toXML();
}

void saveToAsXML(std::ofstream& file, const XMLPrintable& printable) {
    saveTo(file, printable);
}