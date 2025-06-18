/*******************************************************
 *  Frequency-Based Replacement Policy for gem5 (LLC)  *
 *  author:  <your name>                               *
 *  date  :  2025-06-18                                *
 ******************************************************/
#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_FREQ_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_FREQ_RP_HH__

#include "mem/cache/replacement_policies/base.hh"   // BaseReplacementPolicy
#include "base/types.hh"                            // Tick

/** Forward 宣告給 params 系統（SimObject 產生器用） */
struct FreqRPParams;

/**
 * FreqRP – frequency-based (LFU + age tie-break) policy.
 *
 *  - 每 hit 時 freq++（飽和值 UINT32_MAX）
 *  - Victim 先選最小 freq，再選最早 insertTick
 */
class FreqRP : public BaseReplacementPolicy
{
  protected:
    /** 每一個 cache line 伴隨的 meta 資料 */
    struct FreqReplData : ReplacementData
    {
        uint32_t freq;
        Tick     insertTick;

        FreqReplData() : freq(0), insertTick(0) {}
    };

  public:
    typedef FreqRPParams Params;
    explicit FreqRP(const Params *p);
    ~FreqRP() override = default;

    /* BaseReplacementPolicy 介面覆寫 */
    void invalidate(const std::shared_ptr<ReplacementData>& data) const override;
    void touch     (const std::shared_ptr<ReplacementData>& data) const override;
    void reset     (const std::shared_ptr<ReplacementData>& data) const override;
    ReplaceableEntry* getVictim(const ReplacementCandidates& cands) const override;
    std::shared_ptr<ReplacementData> instantiateEntry() override;
};

#endif  // __MEM_CACHE_REPLACEMENT_POLICIES_FREQ_RP_HH__

