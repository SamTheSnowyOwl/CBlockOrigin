importScripts('./cblock_engine.js');

let adblocker;

function mapTypeToBucket(type) {
    if (type === 'script') return 1;
    if (type === 'image' || type === 'images') return 2;
    if (type === 'xmlhttprequest') return 3;
    return 0; // REQ_OTHER
}

async function bootstrapEngine() {
    try {
        const rulesUrl = chrome.runtime.getURL('rules.txt');
        const response = await fetch(rulesUrl);
        const rulesTextData = await response.text();

        let targetModule;
        if (typeof CBlockEngine !== 'undefined') {
            targetModule = CBlockEngine;
        } else if (typeof Module !== 'undefined') {
            targetModule = Module;
        } else if (typeof self.Module !== 'undefined') {
            targetModule = self.Module;
        }

        if (!targetModule) {
            throw new Error("Emscripten core module target could not be found on gloabl scope.");
        }
        const ModuleInstance = (typeof targetModule === 'function') ? await targetModule() : targetModule;

        adblocker = {
            init: ModuleInstance.cwrap('init_adblocker', 'boolean', ['string']),
            check: ModuleInstance.cwrap('should_block_url', 'boolean', ['string', 'number']),
            free: ModuleInstance.cwrap('teardown_adblocker', 'void', [])
        };

        const lines = rulesTextData.split('\n');
        const dnrRules = [];
        const cRulesBuffer = [];
        let ruleId = 1;

        for (let line of lines) {
            line = line.trim();
            if (!line || line.startsWith('!') || line.includes('##')) continue;

            let cleanPattern = line.split('$')[0].replace('||', '').replace('^', '').replace('*', '');
            if (cleanPattern.length < 4) continue;

            dnrRules.push({
                "id": ruleId++,
                "priority": 1,
                "action": { "type": "block" },
                "condition": {
                    "urlFilter": cleanPattern,
                    "resourceTypes": ["script", "image", "xmlhttprequest", "sub_frame"]
                }
            });

            if (cRulesBuffer.length < 1000) {
                cRulesBuffer.push(line);
            }

            if (ruleId > 5000) break;
        }

        const success = adblocker.init(cRulesBuffer.join('\n'));
        if (!success) {
            console.error("Failed to compile rules array inside the local memory layer.");
            return;
        }
        console.log("C-Engine diagnostic structures initialized safely.");

        await chrome.declarativeNetRequest.updateDynamicRules({
            removeRuleIds: Array.from({ length: 5000 }, (_, i) => i + 1),
            addRules: dnrRules
        });

        console.log(`[C-BLOCK SUCCESS] Successfully injected ${dnrRules.length} active blocking rules directly into Vivaldi's native network core.`);
    
    } catch (err) {
        console.error("Critical Failure during adblocker engine bootstrap execution: ", err);
    }
}

bootstrapEngine();

chrome.webRequest.onBeforeRequest.addListener(
    (details) => {
        if (!adblocker) return;

        const url = details.url;
        const bucket = mapTypeToBucket(details.type);

        const isAd = adblocker.check(url, bucket);

        if (isAd) {
            console.warn(`[C-BLOCK ENGINE SEES AD]: ${url}`);
        }        
    },
    { urls: ["<all_urls>"] }    
);