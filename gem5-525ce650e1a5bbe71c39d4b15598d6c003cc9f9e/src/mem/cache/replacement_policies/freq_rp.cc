/*******************************************************
 *  Frequency-Based Replacement Policy – 實作檔         *
 ******************************************************/
#include "mem/cache/replacement_policies/freq_rp.hh"

#include <cassert>
#include <cstdint>
#include <limits>

#include "base/trace.hh"            // DPRINTF 如果想 debug
#include "debug/CacheRepl.hh"       // 對應 debug flag
#include "params/FreqRP.hh"         // SimObject 參數

/** 建構子：只把父類拿來用即可 */
FreqRP::FreqRP(const Params *p) : BaseReplacementPolicy(p) {}

/** 被驅逐或無效化時，freq 清 0、時間戳更新 */
void
FreqRP::invalidate(const std::shared_ptr<ReplacementData>& data) const
{
    auto d = std::static_pointer_cast<FreqReplData>(data);
    d->freq       = 0;
    d->insertTick = curTick();
}

/** Cache hit 時呼叫；freq 上限飽和於 UINT32_MAX */
void
FreqRP::touch(const std::shared_ptr<ReplacementData>& data) const
{
    auto d = std::static_pointer_cast<FreqReplData>(data);
    if (d->freq != std::numeric_limits<uint32_t>::max())
        ++d->freq;
}

/** 新插入的 line：freq = 1，並記錄插入時間 */
void
FreqRP::reset(const std::shared_ptr<ReplacementData>& data) const
{
    auto d = std::static_pointer_cast<FreqReplData>(data);
    d->freq       = 1;
    d->insertTick = curTick();
}

/** 在候選群中選出犧牲者：最小 freq → 最早 insertTick */
ReplaceableEntry*
FreqRP::getVictim(const ReplacementCandidates& cands) const
{
    assert(!cands.empty());

    ReplaceableEntry* victim = cands[0];
    auto vData = std::static_pointer_cast<FreqReplData>(victim->replacementData);

    for (auto& entry : cands) {
        auto eData = std::static_pointer_cast<FreqReplData>(entry->replacementData);

        bool betterFreq = eData->freq < vData->freq;
        bool sameFreqOlder = (eData->freq == vData->freq) &&
                             (eData->insertTick < vData->insertTick);

        if (betterFreq || sameFreqOlder) {
            victim = entry;
            vData  = eData;
        }
    }
    return victim;
}

/** Policy 必須能生成自家 ReplacementData 物件 */
std::shared_ptr<ReplacementData>
FreqRP::instantiateEntry()
{
    return std::make_shared<FreqReplData>();
}
FreqRP*
FreqRPParams::create()
{
    return new FreqRP(this);
}
