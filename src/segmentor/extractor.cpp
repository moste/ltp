#include "extractor.h"
#include "settings.h"

#include "strutils.hpp"
#include "chartypes.hpp"

namespace ltp {
namespace segmentor {

Extractor * Extractor::instance_ = 0;
std::vector<Template *> Extractor::templates;

Extractor * Extractor::extractor() {
    if (0 == instance_) {
        instance_ = new Extractor;
    }

    return instance_;
}

int Extractor::num_templates() {
    if (0 == instance_) {
        instance_ = new Extractor;
    }

    return templates.size();
}

Extractor::Extractor() {
    templates.push_back(new Template("1={c-2}"));
    templates.push_back(new Template("2={c-1}"));
    templates.push_back(new Template("3={c-0}"));
    templates.push_back(new Template("4={c+1}"));
    templates.push_back(new Template("5={c+2}"));
    templates.push_back(new Template("6={c-2}-{c-1}"));
    templates.push_back(new Template("7={c-1}-{c-0}"));
    templates.push_back(new Template("8={c-0}-{c+1}"));
    templates.push_back(new Template("9={c+1}-{c+2}"));
    //templates.push_back(new Template("10={c-2}-{c-0}"));
    //templates.push_back(new Template("11={c-1}-{c+1}"));
    //templates.push_back(new Template("12={c-0}-{c+2}"));
    //templates.push_back(new Template("13={c-1}-{c-0}-{c+1}"));
    templates.push_back(new Template("14={ct-1}"));
    templates.push_back(new Template("15={ct-0}"));
    templates.push_back(new Template("16={ct+1}"));
    templates.push_back(new Template("17={lex1}"));
    templates.push_back(new Template("18={lex2}"));
    templates.push_back(new Template("19={lex3}"));
    //templates.push_back(new Template("17={dup-1}"));
    //templates.push_back(new Template("18={dup-0}"));
    //templates.push_back(new Template("19={dup2-2}"));
    //templates.push_back(new Template("20={dup2-1}"));
    //templates.push_back(new Template("21={dup2-0}"));
}

Extractor::~Extractor() {
    for (int i = 0; i < templates.size(); ++ i) {
        delete templates[i];
    }
}

int Extractor::extract1o(Instance * inst, int idx, std::vector< StringVec > & cache) {
    int len = inst->size();

    Template::Data data;

#define EQU(x, y) (inst->forms[(x)] == inst->forms[(y)])
#define TYPE(x) (strutils::to_str(inst->chartypes[(x)]&0x07))

    data.set( "c-2",    (idx-2 < 0 ? BOS : inst->forms[idx-2]) );
    data.set( "c-1",    (idx-1 < 0 ? BOS : inst->forms[idx-1]) );
    data.set( "c-0",    inst->forms[idx] );
    data.set( "c+1",    (idx+1 >= len ? EOS : inst->forms[idx+1]) );
    data.set( "c+2",    (idx+2 >= len ? EOS : inst->forms[idx+2]) );
    data.set( "ct-1",   (idx-1 < 0 ? BOT : TYPE(idx-1)) );
    data.set( "ct-0",   TYPE(idx) );
    data.set( "ct+1",   (idx+1 >= len ? EOT : TYPE(idx+1)) );
    // data.set( "dup-1",  (idx-1 > 0 && EQU(idx-1, idx) ? "1" : "0") );
    // data.set( "dup-0",  (idx+1 < len && EQU(idx, idx+1) ? "1" : "0") );
    // data.set( "dup2-2", (idx-2 > 0 && EQU(idx-2, idx) ? "1" : "0") );
    // data.set( "dup2-1", (idx-1 > 0 && idx+1 < len && EQU(idx-1, idx+1) ? "1" : "0") );
    // data.set( "dup2-0", (idx+2 < len && EQU(idx, idx+2) ? "1" : "0") );
    data.set( "lex1",   strutils::to_str(inst->lexicon_match_state[idx] & 0x0f));
    data.set( "lex2",   strutils::to_str((inst->lexicon_match_state[idx]>>4) & 0x0f));
    data.set( "lex3",   strutils::to_str((inst->lexicon_match_state[idx]>>8) & 0x0f));

#undef TYPE
#undef EQU

    string feat;
    feat.reserve(1024);
    for (int i = 0; i < templates.size(); ++ i) {
        templates[i]->render(data, feat);
        cache[i].push_back(feat);
    }
    return 0;
}

}       //  end for namespace segmentor
}       //  end for namespace ltp
