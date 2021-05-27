using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class JsonManager : MonoBehaviour
{
    [SerializeField] private string streaminAssetsPath;
    string jsonText;

    private IEnumerator Start()
    {
        streaminAssetsPath = Path.Combine(Application.streamingAssetsPath, "gameData.json");
        StartCoroutine(LoadJson());
        yield return null;
    }

    IEnumerator LoadJson()
    {
        jsonText = string.Empty;
   

        if (File.Exists(streaminAssetsPath))
        {
            jsonText = File.ReadAllText(streaminAssetsPath);
            JsonData jsonData = JsonUtility.FromJson<JsonData>(jsonText);

      

            foreach (var conversation in jsonData.conversation)
            {
                GameInfo.Instance.conversationList.Add(conversation.key, conversation);
            }

            foreach (var character in jsonData.characterInfo)
            {
                GameInfo.Instance.characterInfos.Add(character.name, character);
            }

            foreach (var map in jsonData.mapInfo)
            {
                string[] npcNames = { map.npc_0, map.npc_1, map.npc_2, map.npc_3, map.npc_4 };
                GameInfo.Instance.mapNpcInfos.Add(map.sceneName, npcNames);
            }
        }

        yield return null;
    }

}
