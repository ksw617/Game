using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectPoolManager : MonoBehaviour
{

    public GameObject enemyPrefab;
    public List<EnemyController> enemyControllers;

    public GameObject npcPrefab;
    public List<NpcController> npcControllers;

    private void Awake()
    {
        enemyControllers = new List<EnemyController>();
        npcControllers = new List<NpcController>();
    }

    public Transform GetEnemy(int index)
    {
        Transform childObj;
        foreach (var enemy in enemyControllers)
        {
            if (!enemy.gameObject.activeSelf)
            {
                enemy.gameObject.SetActive(true);
                childObj = enemy.transform.GetChild(index);
                childObj.gameObject.SetActive(true);
                return enemy.transform;
            }
        }

        GameObject obj = Instantiate(enemyPrefab);
        childObj = obj.transform.GetChild(index);
        childObj.gameObject.SetActive(true);
        EnemyController enemyController = obj.GetComponent<EnemyController>();
        enemyControllers.Add(enemyController);
        return obj.transform;

    }

    public Transform GetNPC(int index, out NpcController controller)
    {
        Transform childObj;
        foreach (var npc in npcControllers)
        {
            if (!npc.gameObject.activeSelf)
            {
                controller = npc;
                npc.gameObject.SetActive(true);
                childObj = npc.transform.GetChild(index);
                childObj.gameObject.SetActive(true);
                return npc.transform;
            }
        }

        GameObject obj = Instantiate(npcPrefab);
        childObj = obj.transform.GetChild(index);
        childObj.gameObject.SetActive(true);
        NpcController npcController = obj.GetComponent<NpcController>();
        npcControllers.Add(npcController);
        controller = npcController;
        return obj.transform;

    }

}
