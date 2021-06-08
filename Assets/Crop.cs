using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Crop : MonoBehaviour
{
    public TimeManager timeManager;

    [SerializeField] private int pastTime;
    public int growDate = 4;
    private int childCount;

    private void Start()
    {
        childCount = transform.childCount;   
    }
    private void OnEnable()
    {
        timeManager.changeDay += NextDay;
        pastTime = 0;
    }

    private void NextDay()
    {                       
        pastTime++;
        float percent = (float)pastTime / (float)growDate;
        percent = Mathf.Clamp01(percent);
        int index = Mathf.RoundToInt((childCount - 1) * percent);
        Debug.Log(index);

        if (pastTime > growDate)
        {
            gameObject.SetActive(false); 
        }        
    }

    private void OnDisable()
    {
        timeManager.changeDay -= NextDay;
    }

}
