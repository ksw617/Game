using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class JsonManager : MonoBehaviour
{
    [SerializeField] private string streaminAssetsPath;
    string jsonText;

    private void Start()
    {
        streaminAssetsPath = Path.Combine(Application.streamingAssetsPath, "gameData.json");
        StartCoroutine(LoadJson());
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

            foreach (var npc in jsonData.npc)
            {
                GameInfo.Instance.npcInfos.Add(npc.npcName, npc);
            }
        }

        yield return null;
    }

}
